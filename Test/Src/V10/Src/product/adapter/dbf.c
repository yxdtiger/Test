#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>

#include "dbf.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/* So we can use O_BINARY on non-WIN32 systems. */
#if !defined(O_BINARY) && !defined(_WIN32)
#define O_BINARY (0)
#endif

#if defined(_WIN32)
#define snprintf _snprintf
#define lseek   _lseek
#define ftruncate _chsize
#define read  _read
#pragma warning(disable:4996)
#endif

#define MIN(a, b) ((a)>(b)?(b):(a))

struct dbf_dhead {
	char	dbh_dbt;		      /* memo (dbt) file present */
	char	dbh_date[3];		  /* last update YY, MM, DD */
	char	dbh_records[4];		/* number of records LE */
	char	dbh_hlen[2];		  /* header length LE */
	char	dbh_rlen[2];		  /* record length LE */
	char	dbh_res[20];		  /* padding */
};

#define	DBH_DATE_YEAR	  0		/* byte offset for year in dbh_date */
#define	DBH_DATE_MONTH	1
#define	DBH_DATE_DAY	  2

/*
 * field description on disk
 */

struct dbf_dfield {
	char	dbf_name[DBF_NAMELEN];	/* name of field */
	char	dbf_type;		/* type of field */
	char	dbf_fda[4];		/* something for dbase III */
	char	dbf_flen[2];		/* field length [and decimal if N] */
	char	dbf_res[14];		/* padding */
};

#define	VALID_RECORD	' '
#define	DELETED_RECORD	'*'

#define EOF_MARKER 0x1A

static const char *dbf_last_error = DBF_E_NONE;

static dbhead_t *get_dbf_head(int fd);
static void free_dbf_head(dbhead_t *dbh);
static int get_dbf_field(dbhead_t *dbh, dbfield_t *dbf);
static int get_piece(dbhead_t *dbh, long offset, char *cp, int len);
static int put_piece(dbhead_t *dbh, long offset, char *cp, int len);
static size_t strlcpy(char *, const char *, size_t);

static void put_long(char *cp, long lval);
static long get_long(char *cp);
static int get_short(char *cp);
static void put_short(char *cp, int ival);
static void put_double(char *cp, double dval);
static double get_double(char *cp);

static void copy_fill(char *dp, const char *sp, int len);
static void copy_crimp(char *dp, const char *sp, int len);
static void db_set_date(char *cp, int year, int month, int day);
static char *db_cur_date(char *cp);
static int db_date_year(const char *cp);
static int db_date_month(const char *cp);
static int db_date_day(const char *cp);
static char *get_dbf_f_fmt(dbfield_t *dbf);

static int mark_dbf_end(dbhead_t *dbh);
static int put_dbf_head(dbhead_t *dbh);
static int dbf_truncate_(dbhead_t *dbh, const char *file);

#define check_field_id(dbh, idx) assert(0 <= idx && idx < dbh->db_nfields);

#define check_field_type(dbh, idx, types) \
  assert(0 <= idx && idx < dbh->db_nfields && NULL != strchr(types, dbh->db_fields[idx].db_type));

#define check_rec_num(dbh, rec_num) \
  (1 <= rec_num && rec_num <= dbh->db_records)

#define check_cache_hit(dbh, rec_num) \
  (dbh->cache_beg <= rec_num && rec_num <= dbh->cache_end)

#define set_cursor(dbh, rec_num) {                                             \
  assert(check_cache_hit(dbh, rec_num));                                       \
  dbh->db_cur_rec = rec_num;                                                   \
  dbh->db_cur_data = dbh->cache + dbh->db_rlen*(dbh->db_cur_rec - dbh->cache_beg);\
}

#define cache_is_append(dbh) (dbh->cache_beg >= dbh->db_records)

static int cache_prefetch(dbhead_t *dbh, int rec_num) {
  int size = dbh->cache_size, beg, end, len;
  int rlen = dbh->db_rlen;

  assert(!check_cache_hit(dbh, rec_num));
  assert(check_rec_num(dbh, rec_num));

  beg = rec_num;
  end = MIN(rec_num + size - 1, dbh->db_records);
  len = (end - beg + 1) * rlen;
  if (len != get_piece(dbh, dbh->db_hlen + rlen * (beg - 1), dbh->cache, len)) {
    dbf_last_error = DBF_E_READ;
    return 0;
  }

  dbh->cache_beg = beg;
  dbh->cache_end = end;
  return 1;
}

static int cache_writeback(dbhead_t *dbh) {
  int len, rlen;
  long offset;

  rlen = dbh->db_rlen;
  len = (dbh->cache_end - dbh->cache_beg + 1) * rlen;
  offset = dbh->db_hlen + (dbh->cache_beg - 1) * rlen;

  if (len != put_piece(dbh, offset, dbh->cache, len)) {
    dbf_last_error = DBF_E_WRITE;
    return 0;
  }

  if (cache_is_append(dbh)) {
    dbh->db_records = dbh->cache_end;
    if (!put_dbf_head(dbh) || !mark_dbf_end(dbh)) {
      dbf_last_error = DBF_E_WRITE;
      return 0;
    }
  }
  dbh->cache_dirty = 0;
  return 1;
}

static int cache_init(dbhead_t *dbh, int size) {
  int len = dbh->db_rlen * size;
  assert(dbh->cache == NULL);
  if ((dbh->cache = (char*) malloc(len)) == NULL) {
    dbf_last_error = DBF_E_MEM;
    return 0;
  }
  memset(dbh->cache, ' ', len);
  dbh->cache_size = size;
  return 1;
}

static void cache_reset(dbhead_t *dbh) {
  dbh->cache_beg = 0;
  dbh->cache_end = 0;
  dbh->cache_dirty = 0;
  dbh->db_cur_rec = 0;
  dbh->db_cur_data = NULL;
}

static int cache_reserved(dbhead_t *dbh, int size) {
  int init = 1;
  if (dbh->cache == NULL) {
    init = cache_init(dbh, size);
  } else if (size > dbh->cache_size) {
    free(dbh->cache);
    dbh->cache = NULL;
    init = cache_init(dbh, size);
  }
  return init;
}


/*
 * get the header info from the file
 *	basic header info & field descriptions
 */
static dbhead_t *get_dbf_head(int fd)
{
	dbhead_t *dbh;
	struct dbf_dhead  dbhead;
	dbfield_t *dbf, *cur_f, *tdbf;
	int ret, nfields, offset, gf_retval;

	if ((dbh = (dbhead_t *)calloc(1, sizeof(dbhead_t))) == NULL)
		return NULL;
	if (lseek(fd, 0, 0) < 0) {
		free(dbh);
		return NULL;
	}
	if ((ret = read(fd, &dbhead, sizeof(dbhead))) <= 0) {
    dbf_last_error = DBF_E_READ;
		free(dbh);
		return NULL;
	}

  if ((dbhead.dbh_dbt & 0x07) != DBH_TYPE_NORMAL) {
    dbf_last_error = DBF_E_FILE;
    free(dbh);
    return NULL;
  }

	/* build in core info */
	dbh->db_fd = fd;
	dbh->db_dbt = dbhead.dbh_dbt;
	dbh->db_records = get_long(dbhead.dbh_records);
	dbh->db_hlen = get_short(dbhead.dbh_hlen);
	dbh->db_rlen = get_short(dbhead.dbh_rlen);

	db_set_date(dbh->db_date, dbhead.dbh_date[DBH_DATE_YEAR] + 1900,
		dbhead.dbh_date[DBH_DATE_MONTH],
		dbhead.dbh_date[DBH_DATE_DAY]);

	/* malloc enough memory for the maximum number of fields:
	   32 * 1024 = 32K dBase5 (for Win) seems to allow that many */
	tdbf = (dbfield_t *)calloc(1, sizeof(dbfield_t)*1024);

	offset = 1;
	nfields = 0;
	gf_retval = 0;
	for (cur_f = tdbf; gf_retval < 2 && nfields < 1024; cur_f++) {
		gf_retval = get_dbf_field(dbh, cur_f);

		if (gf_retval < 0) {
			free_dbf_head(dbh);
			free(tdbf);
			return NULL;
		}
		if (gf_retval != 2 ) {
			cur_f->db_foffset = offset;
			offset += cur_f->db_flen;
			nfields++;
		}
	}
	dbh->db_nfields = nfields;

	/* malloc the right amount of space for records, copy and destroy old */
	dbf = (dbfield_t *)malloc(sizeof(dbfield_t)*nfields);
	memcpy(dbf, tdbf, sizeof(dbfield_t)*nfields);
	free(tdbf);

	dbh->db_fields = dbf;

	return dbh;
}

/*
 * free up the header info built above
 */
static void free_dbf_head(dbhead_t *dbh)
{
	dbfield_t *dbf, *cur_f;
	int nfields;

	dbf = dbh->db_fields;
	nfields = dbh->db_nfields;
	for (cur_f = dbf; cur_f < &dbf[nfields]; cur_f++) {
		if (cur_f->db_format) {
			free(cur_f->db_format);
		}
	}

	free(dbf);
	free(dbh);
}

/*
 * put out the header info
 */
static int put_dbf_head(dbhead_t *dbh)
{
	int fd = dbh->db_fd;
	struct dbf_dhead dbhead;
	int	ret;

	memset (&dbhead, 0, sizeof(dbhead));

	/* build on disk info */
	dbhead.dbh_dbt = dbh->db_dbt;
	put_long(dbhead.dbh_records, dbh->db_records);
	put_short(dbhead.dbh_hlen, dbh->db_hlen);
	put_short(dbhead.dbh_rlen, dbh->db_rlen);

	/* put the date spec'd into the on disk header */
	dbhead.dbh_date[DBH_DATE_YEAR] =(char)(db_date_year(dbh->db_date) -
						1900);
	dbhead.dbh_date[DBH_DATE_MONTH]=(char)(db_date_month(dbh->db_date));
	dbhead.dbh_date[DBH_DATE_DAY] =(char)(db_date_day(dbh->db_date));

	if (lseek(fd, 0, 0) < 0)
		return -1;
	if ((ret = write(fd, &dbhead, sizeof(dbhead))) <= 0)
		return -1;
	return ret;
}

/*
 * get a field off the disk from the current file offset
 */
static int get_dbf_field(dbhead_t *dbh, dbfield_t *dbf)
{
	struct dbf_dfield	dbfield;
	int ret;

	if ((ret = read(dbh->db_fd, &dbfield, sizeof(dbfield))) <= 0) {
		return ret;
	}

	/* Check for the '0Dh' field terminator , if found return '2'
	   which will tell the loop we are at the end of fields */
	if (dbfield.dbf_name[0]==0x0d) {
		return 2;
	}

	/* build the field name */
	copy_crimp(dbf->db_fname, dbfield.dbf_name, DBF_NAMELEN);

	dbf->db_type = dbfield.dbf_type;
	switch (dbf->db_type) {
	  case 'N':
		dbf->db_flen = dbfield.dbf_flen[0];
		dbf->db_fdc = dbfield.dbf_flen[1];
		break;
    case 'D':
		dbf->db_flen = 8;
		break;
	  case 'L':
		dbf->db_flen = 1;
		break;
    case 'M':
    dbf->db_flen = 10;
    break;
	  default:
	    	dbf->db_flen = get_short(dbfield.dbf_flen);
		break;
	}

	if ((dbf->db_format = get_dbf_f_fmt(dbf)) == NULL) {
		/* something went wrong, most likely this fieldtype is not supported */
    dbf_last_error = DBF_E_NOTSUPPORT;
		return -1;
	}

	return 0;
}

/*
 * put a field out on the disk at the current file offset
 */
static int put_dbf_field(dbhead_t *dbh, dbfield_t *dbf)
{
	struct dbf_dfield	dbfield;
	char			*scp, *dcp;
	int			ret;

	memset (&dbfield, 0, sizeof(dbfield));

	/* build the on disk field info */
	scp = dbf->db_fname; dcp = dbfield.dbf_name;

	strlcpy(dbfield.dbf_name, dbf->db_fname, DBF_NAMELEN + 1);

	dbfield.dbf_type = dbf->db_type;
	switch (dbf->db_type) {
    case 'N':
		dbfield.dbf_flen[0] = dbf->db_flen;
		dbfield.dbf_flen[1] = dbf->db_fdc;
		break;
    default:
	    put_short(dbfield.dbf_flen, dbf->db_flen);
	}

	/* now write it out to disk */
	if ((ret = write(dbh->db_fd, &dbfield, sizeof(dbfield))) <= 0) {
    dbf_last_error = DBF_E_WRITE;
		return ret;
	}
	return 1;
}

/*
 * put out all the info at the top of the file...
 */
static char end_stuff[2] = {0x0d, 0};

static void put_dbf_info(dbhead_t *dbh)
{
	dbfield_t	*dbf;
	char		*cp;
	int		fcnt;

	if ((cp = db_cur_date(NULL))) {
		strlcpy(dbh->db_date, cp, 9);
		free(cp);
	}
	put_dbf_head(dbh);
	dbf = dbh->db_fields;
	for (fcnt = dbh->db_nfields; fcnt > 0; fcnt--, dbf++)
		put_dbf_field(dbh, dbf);
	write(dbh->db_fd, end_stuff, 1);
}

static char *get_dbf_f_fmt(dbfield_t *dbf) {
	char format[100];

	/* build the field format for printf */
	switch (dbf->db_type) {
    case 'C':
		snprintf(format, sizeof(format), "%%-%ds", dbf->db_flen);
		break;
    case 'N': case 'D': case 'L':
        snprintf(format, sizeof(format), "%%%d.%df", dbf->db_flen, dbf->db_fdc);
    	break;
    case 'M':
		strlcpy(format, "%d", sizeof(format));
		break;
	  default:
      dbf_last_error = DBF_E_NOTSUPPORT;
		  return NULL;
	}
	return (char *)strdup(format);
}

void dbf_head_info(dbhead_t *dbh)
{
	int	nfields;
	dbfield_t *dbf, *cur_f;

	nfields = dbh->db_nfields;
	printf("# fields: %d, record len: %d, total records %ld\n",
		nfields, dbh->db_rlen, dbh->db_records);
	dbf = dbh->db_fields;
	for (cur_f = dbf; cur_f < &dbf[nfields] ; cur_f++) {
		printf("# %s, %c, %d, %d\n", cur_f->db_fname,
			cur_f->db_type, cur_f->db_flen, cur_f->db_fdc);
	}
}

/*
 * get a record off the database
 */
static int get_dbf_record(dbhead_t *dbh, int rec_num, char *buf, size_t size) {
	long	offset;

  assert(buf != NULL);
  assert(size >= dbh->db_rlen);

  if (rec_num > dbh->db_records) {
		return 0;
	}

	/* go to the correct spot on the file */
	offset = dbh->db_hlen + (rec_num - 1) * dbh->db_rlen;
	if (get_piece(dbh, offset, buf, dbh->db_rlen) != dbh->db_rlen)
		return 0;

	dbh->db_cur_rec = rec_num;
	return 1;
}

static int get_piece(dbhead_t *dbh, long offset, char *cp, int len) {
	/* go to the correct spot on the file */
	if ( lseek(dbh->db_fd, offset, 0) < 0 ) {
		return -1;
	}

	/* read the record into the allocated space */
	return read(dbh->db_fd, cp, len);
}

/*
 * put a record to the database
 */
static int put_dbf_record(dbhead_t *dbh, int rec_num, char *cp) {
	int	offset;

	if (rec_num == 0) {
		rec_num = dbh->db_records;
	}
	if (rec_num > dbh->db_records) {
		return 0L;
	}
	/* go to the correct spot on the file */
	offset = dbh->db_hlen + (rec_num - 1) * dbh->db_rlen;
	if (put_piece(dbh, offset, cp, dbh->db_rlen) != dbh->db_rlen) {
		rec_num = -1;
	}
  //if (rec_num == dbh->db_records && mark_dbf_end(dbh)) {
	return rec_num;
}

static int put_piece(dbhead_t *dbh, long offset, char *cp, int len) {
	/* go to the correct spot on the file */
	if ( lseek(dbh->db_fd, offset, 0) < 0 ) {
		return -1;
	}

	/* write the record into the file */
	return write(dbh->db_fd, cp, len);
}

static int mark_dbf_end(dbhead_t *dbh) {
  long offset; char cp = EOF_MARKER;
  offset = dbh->db_hlen + dbh->db_records * dbh->db_rlen;
  return put_piece(dbh, offset, &cp, 1);
}

static int del_dbf_record(dbhead_t *dbh, long rec_num) {
  int offset;
	char cp = DELETED_RECORD;
	if (rec_num > dbh->db_records)
		return -1;

  offset = dbh->db_hlen + (rec_num - 1) * dbh->db_rlen;
  return put_piece(dbh, offset, &cp, 1);
}

void pack_dbf(dbhead_t *dbh) {
	long	out_off, in_off;
	int	rec_cnt, new_cnt;
	char	*cp;

	if ((cp = (char *)malloc(dbh->db_rlen)) == NULL) {
		return;
	}
	in_off = out_off = dbh->db_hlen;

	new_cnt = 0;
	rec_cnt = dbh->db_records;
	while (rec_cnt > 0) {
		if (get_piece(dbh, in_off, cp, dbh->db_rlen) < 0)
			break;

		if (*cp != DELETED_RECORD) {
			/* write the record into the file */
			if (put_piece(dbh, out_off, cp, dbh->db_rlen) < 0)
				break;
			out_off += dbh->db_rlen;
			new_cnt++;
		}
		in_off += dbh->db_rlen;
		rec_cnt--;
	}
	free(cp);

	/* Try to truncate the file to the right size. */
	if (ftruncate(dbh->db_fd, out_off) != 0) {
	    printf ("dbase_pack() couldn't truncate the file to the right size. Some deleted records may still be left in there.");
	}

	if (rec_cnt == 0)
		dbh->db_records = new_cnt;

  mark_dbf_end(dbh);
}

/* routine to get a field from a record */
static size_t get_field_val(char *rp, dbfield_t *fldp, char *buf, size_t size) {
	int flen = fldp->db_flen;
  assert(size >= flen + 1);
  flen = (int)(MIN(flen, size));
  memcpy(buf, rp + fldp->db_foffset, flen);
  buf[flen] = '\0';
	return flen;
}

static void put_field_val(char *rp, dbfield_t *fldp, char *cp) {
	strncpy(&rp[fldp->db_foffset], cp, fldp->db_flen);
}

/*
 * output a record for [DEBUG]
 */
void dbf_print(dbhead_t *dbh) {
	dbfield_t *cur_f;
	int nfields = dbh->db_nfields;
  dbfield_t *dbf = dbh->db_fields;
  char *cp = dbh->db_cur_data;
  char buf[256];
  assert(cp != NULL);
	printf("%c", *cp);
	for (cur_f = dbf; cur_f < &dbf[nfields] ; cur_f++) {
		printf(" ");
    get_field_val(cp, cur_f, buf, 256);
		printf(cur_f->db_format, buf);
	}
	printf("\n");
}

/* check for record validity */
static int is_valid_rec(char *cp) {
  if (cp && (*cp != DELETED_RECORD))
		return 1;
	else
		return 0;
}

//cursor operators
/* reset cursor to first record */
int dbf_reset(dbhead_t *dbh) {
  if (cache_reserved(dbh, dbh->db_rlen > CACHE_SIZE? 1 : CACHE_SIZE/dbh->db_rlen)) {
    cache_reset(dbh);
    return 1;
  }
  return 0;
}

/* get the next valid record */
int dbf_next(dbhead_t *dbh) {
  int rec_num;
  if (dbh->cache == NULL)
    return 0;

  do {
    rec_num = dbh->db_cur_rec + 1;
    if (!check_cache_hit(dbh, rec_num)) {
      if (!((dbh->cache_dirty && !cache_writeback(dbh)) ||
        check_rec_num(dbh, rec_num) && cache_prefetch(dbh, rec_num)))
        return 0;
    }
    set_cursor(dbh, rec_num);
  } while(!is_valid_rec(dbh->db_cur_data));
  return rec_num;
}

/* get the next record */
int dbf_next_record(dbhead_t *dbh) {
  int rec_num;
  if (dbh->cache == NULL)
    return 0;

  rec_num = dbh->db_cur_rec + 1;
  if (dbf_moveto(dbh, rec_num))
    return rec_num;
  return 0;
}

int dbf_moveto(dbhead_t *dbh, int rec_num) {
  if (!check_rec_num(dbh, rec_num))
    return 0;

  if (!check_cache_hit(dbh, rec_num)) {
    if (!((dbh->cache_dirty && !cache_writeback(dbh)) || cache_prefetch(dbh, rec_num)))
      return 0;
  }
  set_cursor(dbh, rec_num);
  return 1;
}

#define mark_cache_dirty(dbh) dbh->cache_dirty = 1;

int dbf_append(dbhead_t *dbh, int n) {
  if (cache_reserved(dbh, n)) {
    dbh->db_cur_data = NULL;
    dbh->db_cur_rec = dbh->db_records;
    dbh->cache_beg = dbh->db_records + 1;
    dbh->cache_end = dbh->db_records + n;
    return 1;
  }
  return 0;
}

int dbf_delete(dbhead_t *dbh, int rec_num) {
  return del_dbf_record(dbh, rec_num);
}

int dbf_flush(dbhead_t *dbh) {
  if (!dbh->cache_dirty)
    return 0;
  return cache_writeback(dbh);
}

//dbf field descriptor parser
static int dbf_field_name(dbfield_t *f, const char **s) {
  const char *s1 = *s;
  while (*s1 == ' ') {
    s1++; (*s)++;
  }
  while (*s1 && *s1 != ':')
    s1++;
  if (!*s || s1 == *s || s1 - *s > 11) {
    dbf_last_error = DBF_E_FNAME;
    return 0;
  }
  memcpy(f->db_fname, *s, s1-*s);
  f->db_fname[s1-*s] = '\0';
  *s = s1 + 1;
  return 1;
}

static int dbf_field_default_len(int t) {
  switch (t) {
  case 'D':
	  return 8;
	case 'L':
	  return 1;
  case 'M':
    return 10;
  default:
    dbf_last_error = DBF_E_NOTSUPPORT;
    return 0;
  }
}

static int dbf_field_type(dbfield_t *f, const char **s) {
  const char *s1 = *s;
  while (*s1 == ' ') {
    s1++;
  }
  if (!strchr("NDCLM", toupper(*s1))) {
    dbf_last_error = DBF_E_FTYPE;
    return 0;
  }
  f->db_type = toupper(*s1);
  s1++;
  while (*s1 == ' ')
    s1++;
  *s = s1;
  return 1;
}

static int dbf_field_len(dbfield_t *f, const char **s) {
  const char *s1 = *s; int len = 0;
  if (!strchr("NC", f->db_type)) {
    f->db_flen = dbf_field_default_len(f->db_type);
    return 1;
  }
  f->db_flen = 0;
  if (*s1++ != ',') {
    dbf_last_error = DBF_E_COMMA;
    return 0;
  }
  while (*s1 == '0' || *s1 == ' ')
    s1++;
  while (isdigit(*s1)) {
    if (f->db_flen == 0)
      f->db_flen = *s1 - '0';
    else
      f->db_flen = f->db_flen*10 + (*s1 - '0');
    s1++;
  }
  while (*s1 && *s1 == ' ')
    s1++;
  *s = s1;
  return 1;
}

static int dbf_field_dec(dbfield_t *f, const char **s) {
  const char *s1 = *s; int dec = 0;
  f->db_fdc = 0;
  if (!strchr("N", f->db_type)) {
    return 1;
  }
  if (*s1++ != ',') {
    dbf_last_error = DBF_E_COMMA;
    return 0;
  }
  while (*s1 == '0' || *s1 == ' ')
    s1++;
  while (isdigit(*s1)) {
    if (f->db_fdc == 0)
      f->db_fdc = *s1 - '0';
    else
      f->db_fdc = f->db_fdc*10 + (*s1 - '0');
    s1++;
  }
  if (f->db_fdc > f->db_flen) {
    dbf_last_error = DBF_E_FDEC_TOOLONG;
    return 0;
  }
  while (*s1 && *s1 == ' ')
    s1++;
  *s = s1;
  return 1;
}

static int dbf_field_semicolon(dbfield_t *f, const char **s) {
  const char *s1 = *s;
  while (*s1 == ' ')
    s1++;
  if (*s1 != '\0' && *s1 != ';') {
    dbf_last_error = DBF_E_SEMICOLON;
    return 0;
  }
  if (*s1 != '\0')
    s1++;
  *s = s1;
  return 1;
}

// field_desc = name:spec{; name:spec}
// spec = type[, len, dec]
static int dbf_parse_field_desc(dbhead_t *dbh, const char *field_desc) {
  int cnt = 0, reserved = 0;
  dbfield_t *fields = NULL, field0;
  const char *s = field_desc;
  int rlen = 1;

  while (*s) {
    if (dbf_field_name(&field0, &s) &&
        dbf_field_type(&field0, &s) &&
        dbf_field_len(&field0, &s) &&
        dbf_field_dec(&field0, &s) &&
        dbf_field_semicolon(&field0, &s)) {
          if (cnt >= reserved) {
            if (reserved == 0) {
              reserved = 8;
              fields = (dbfield_t*) malloc(sizeof(dbfield_t)*reserved);
            } else {
              reserved *= 2;
              fields = (dbfield_t*) realloc(fields, sizeof(dbfield_t)*reserved);
            }
            if (NULL == fields) {
              dbf_last_error = DBF_E_MEM;
              return 0;
            }
          }
          field0.db_foffset = rlen;
          rlen += field0.db_flen;
          field0.db_format = get_dbf_f_fmt(&field0);
          memcpy(fields + cnt, &field0, sizeof(dbfield_t));
          cnt ++;
    } else {
      break;
    }
  }
  if (*s || fields == NULL) {
    if (!fields)
      free(fields);
    return 0;
  }

  dbh->db_nfields = cnt;
  if (NULL == (dbh->db_fields = (dbfield_t*) malloc(sizeof(dbfield_t)*cnt))) {
    dbf_last_error = DBF_E_MEM;
    free(fields);
    return 0;
  }
  memcpy(dbh->db_fields, fields, sizeof(dbfield_t)*cnt);
  free(fields);
  dbh->db_rlen = rlen;
  dbh->db_hlen = sizeof(struct dbf_dhead) + 1 + sizeof(struct dbf_dfield)*dbh->db_nfields;
  return 1;
}

static int dbf_check_support(dbhead_t *dbh) {
  int i;
  for (i = 0; i < dbh->db_nfields; i++) {
    if (strchr("M", dbh->db_fields[i].db_type)) {
      dbf_last_error = DBF_E_MEMO_NOT_SUPPORT;
      return 0;
    }
  }
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
const char* dbf_error() {
  return dbf_last_error;
}
//dbf file operator
dbhead_t *dbf_new(const char *file, const char* field_desc) {
  dbhead_t *dbh = (dbhead_t*) malloc(sizeof(dbhead_t));
  memset(dbh, 0, sizeof(dbhead_t));
  dbh->db_fd = -1;
  dbh->db_name = NULL;
  dbh->db_dbt = DBH_TYPE_NORMAL;
  dbh->cache = NULL;
  db_cur_date(dbh->db_date);

  if (!dbf_parse_field_desc(dbh, field_desc) || !dbf_check_support(dbh)) {
    free(dbh);
    return NULL;
  }

  if (file != NULL && !dbf_truncate_(dbh, file)) {
    dbf_destroy(dbh);
    return NULL;
  }
  dbf_reset(dbh);
  return dbh;
}

dbhead_t *dbf_open(const char *dp, int o_flags) {
	int fd;
	const char *cp;
	dbhead_t *dbh;

	cp = dp;
	if ((fd = open(cp, o_flags|O_BINARY)) < 0) {
    dbf_last_error = DBF_E_OPEN;
		return NULL;
	}

	if ((dbh = get_dbf_head(fd)) ==	NULL) {
    close(fd);
		return NULL;
	}

  dbh->cache = NULL;
  dbf_reset(dbh);
	return dbh;
}

//截断操作
static int dbf_truncate_(dbhead_t *dbh, const char* file) {
  if (file != NULL) {
    int fd;
    if ((fd = open(file, O_BINARY|O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1) {
      dbf_last_error = DBF_E_OPEN;
      return 0;
    }
    if (dbh->db_fd != -1) {
      close(dbh->db_fd);
      free(dbh->db_name);
    }
    dbh->db_fd = fd;
    dbh->db_name = strdup(file);
  }
  if (dbh->db_fd == -1) {
    dbf_last_error = DBF_E_INVALID_FILE_HANDLE;
    return 0;
  }

  if (ftruncate(dbh->db_fd, 0) != 0)  {
    dbf_last_error = DBF_E_WRITE;
    return 0;
  }
  
  dbh->db_records = 0;
  put_dbf_info(dbh);
  mark_dbf_end(dbh);
  cache_reset(dbh);
  return 1;
}

int dbf_truncate(dbhead_t *dbh) {
  assert(dbh->db_fd != -1);
  return dbf_truncate_(dbh, NULL);
}

void dbf_destroy(dbhead_t *dbh) {
  dbf_flush(dbh);

  if (dbh->db_fd != -1) {
    close(dbh->db_fd);
    dbh->db_fd = -1;
  }
  if (dbh->db_name != NULL) {
    free(dbh->db_name);
    dbh->db_name = NULL;
  }
  if (dbh->cache != NULL) {
    free(dbh->cache);
    dbh->cache = NULL;
  }
  free_dbf_head(dbh);
}

char dbf_get_flag(dbhead_t *dbh) {
  return *dbh->db_cur_data;
}

void dbf_set_flag(dbhead_t *dbh, char flag) {
  *dbh->db_cur_data = flag;
  mark_cache_dirty(dbh);
}

//accessors
//返回原始串，结束符用0填充
size_t dbf_get_val(dbhead_t *dbh, int idx, char *buf, size_t size) {
  check_field_id(dbh, idx);
  return get_field_val(dbh->db_cur_data, &dbh->db_fields[idx], buf, size);
}

#ifndef strtoi
static int strtoi(const char *str, char **endp) {
  int n = 0, pre = 0; int sign = 1;
  const char *p = str;
  assert(p);
  for (; isspace(*p); ++p) continue;
  if (*p == '-') { sign = -1; ++p; }
  else if (*p == '+') { ++p; }
  if (isdigit(*p)) {
    for (; *p == '0'; ++p) continue;
    for (; isdigit(*p); ++p) {
      pre = n;
      n = n*10 + (*p - '0');
      if (n < pre) { //overflow
        --p;
        break;
      }
    }
    if (*p == '.') {
      ++p;
      while (isdigit(*p)) ++p;
    }
    *endp = (char*) p;
    return n * sign;
  }
  *endp = (char*) str;
  return luai_NI;
}
#endif

//advance data type support
int dbf_get_int(dbhead_t *dbh, int idx) {
  char buf[32], *endp;
  check_field_type(dbh, idx, "N");
  dbf_get_val(dbh, idx, buf, 32);
  return strtoi(buf, &endp);;
}

double dbf_get_num(dbhead_t *dbh, int idx) {
  char buf[32], *endp; double n;
  check_field_type(dbh, idx, "N");
  dbf_get_val(dbh, idx, buf, 32);
  n = strtod(buf, &endp);
  return *endp||endp==buf?luai_NN:n;
}

#ifndef strtolld
static long long strtolld(const char *str, char **endp) {
  long long lld = 0, pre = 0; int sign = 1;
  const char *p = str;
  assert(p);
  for (p = str; isspace(*p); ++p) continue;
  if (*p == '-') { sign = -1; ++p; }
  else if (*p == '+') { ++p; }
  if (isdigit(*p)) {
    for (; *p == '0'; ++p) continue;
    for (; isdigit(*p); ++p) {
      lld = pre*10 + (*p - '0');
      if (lld < pre) { //overflow
        --p;
        break;
      } else {
        pre = lld;
      }
    }
    if (*p == 'L' || *p == 'l') ++p;
    else if (*p == '.') {
      ++p;
      while (isdigit(*p)) ++p;
    }
    *endp = (char*) p;
    return lld * sign;
  }
  *endp = (char*) str;
  return luai_NL;
}
#endif

long long dbf_get_long(dbhead_t *dbh, int idx) {
  char buf[32], *endp;
  check_field_type(dbh, idx, "N");
  dbf_get_val(dbh, idx, buf, 32);
  return strtolld(buf, &endp);
}

char dbf_get_logical(dbhead_t *dbh, int idx) {
  char buf[2];
  check_field_type(dbh, idx, "L");
  get_field_val(dbh->db_cur_data, &dbh->db_fields[idx], buf, 2);
  return buf[0];
}

char *dbf_get_string(dbhead_t *dbh, int idx, char *ret, size_t size) {
  check_field_type(dbh, idx, "CDNML");
  if (ret) {
    size_t i = get_field_val(dbh->db_cur_data, &dbh->db_fields[idx], ret, size);
    while (i > 0 && ret[i-1] == ' ')
      ret[--i] = '\0';
    return ret;
  } else {
    char buf[256];
    size_t i = get_field_val(dbh->db_cur_data, &dbh->db_fields[idx], buf, 256);
    while (i > 0 && buf[i-1] == ' ')
      buf[--i] = '\0';
    return strdup(buf);
  }
}

int dbf_get_date(dbhead_t *dbh, int idx) {
  char buf[9], *endp; int n;
  check_field_type(dbh, idx, "D");
  get_field_val(dbh->db_cur_data, &dbh->db_fields[idx], buf, 9);
  n = (int)strtod(buf, &endp);
  return *endp?luai_NI:n;
}

void dbf_set_val(dbhead_t *dbh, int idx, const char *buf) {
  dbfield_t *field;
  check_field_id(dbh, idx);
  field = &dbh->db_fields[idx];
  copy_fill(dbh->db_cur_data + field->db_foffset, buf, field->db_flen);
  mark_cache_dirty(dbh);
}

#define dbf_set_empty(dbh, idx) dbf_set_val(dbh, idx, "")

void dbf_set_int(dbhead_t *dbh, int idx, int val) {
  dbfield_t *field;
  char buf[256];
  check_field_type(dbh, idx, "N");
  if (isNaI(val)) {
    dbf_set_empty(dbh, idx);
  } else {
    field = &dbh->db_fields[idx];
    snprintf(buf, sizeof buf, field->db_format, (double)val);
    copy_fill(dbh->db_cur_data + field->db_foffset, buf, field->db_flen);
  }
  mark_cache_dirty(dbh);
}

void dbf_set_long(dbhead_t *dbh, int idx, long long val) {
  dbfield_t *field;
  char buf[256];
  check_field_type(dbh, idx, "N");
  if (isNaL(val)) {
    dbf_set_empty(dbh, idx);
  } else {
    field = &dbh->db_fields[idx];
    snprintf(buf, sizeof buf, field->db_format, (double)val);
    copy_fill(dbh->db_cur_data + field->db_foffset, buf, field->db_flen);
  }
  mark_cache_dirty(dbh);
}

void dbf_set_num(dbhead_t *dbh, int idx, double val) {
  dbfield_t *field;
  char buf[256];
  check_field_type(dbh, idx, "N");
  if (isNaN(val)) {
    dbf_set_empty(dbh, idx);
  } else {
    field = &dbh->db_fields[idx];
    snprintf(buf, sizeof buf, field->db_format, val);
    copy_fill(dbh->db_cur_data + field->db_foffset, buf, field->db_flen);
  }
  mark_cache_dirty(dbh);
}

void dbf_set_string(dbhead_t *dbh, int idx, const char *str) {
  dbfield_t *field;
  char buf[256];
  check_field_type(dbh, idx, "C");
  field = &dbh->db_fields[idx];
  snprintf(buf, sizeof buf, field->db_format, str);
  copy_fill(dbh->db_cur_data + field->db_foffset, buf, field->db_flen);
  mark_cache_dirty(dbh);
}

void dbf_set_date(dbhead_t *dbh, int idx, int yyyymmdd) {
  dbfield_t *field;
  char buf[16];
  check_field_type(dbh, idx, "D");
  if (isNaI(yyyymmdd)) {
    dbf_set_empty(dbh, idx);
  } else {
    field = &dbh->db_fields[idx];
    snprintf(buf, sizeof buf, field->db_format, yyyymmdd);
    copy_fill(dbh->db_cur_data + field->db_foffset, buf, field->db_flen);
  }
  mark_cache_dirty(dbh);
}

void dbf_set_logical(dbhead_t *dbh, int idx, char ch) {
  check_field_type(dbh, idx, "L");
  assert(ch == 'T' || ch == 'F' || ch == 0x20);
  dbh->db_cur_data[dbh->db_fields[idx].db_foffset] = ch;
  mark_cache_dirty(dbh);
}
////////////////////////////////////////////////////////////////////////////////
//misc
/*
 * routine to change little endian long to host long
 */
static long get_long(char *cp) {
	int ret;
	unsigned char *source = (unsigned char *)cp;

	ret = *source++;
	ret += ((*source++)<<8);
	ret += ((*source++)<<16);
	ret += ((*source++)<<24);

	return ret;
}

static void put_long(char *cp, long lval) {
	*cp++ = lval & 0xff;
	*cp++ = (lval >> 8) & 0xff;
	*cp++ = (lval >> 16) & 0xff;
	*cp++ = (lval >> 24) & 0xff;
}

/*
 * routine to change little endian short to host short
 */
static int get_short(char *cp) {
	int ret;
	unsigned char *source = (unsigned char *)cp;

	ret = *source++;
	ret += ((*source++)<<8);

	return ret;
}

static void put_short(char *cp, int sval) {
	*cp++ = sval & 0xff;
	*cp++ = (sval >> 8) & 0xff;
}

static double get_double(char *cp) {
	double ret;
	unsigned char *dp = (unsigned char *)&ret;

	dp[7] = *cp++;
	dp[6] = *cp++;
	dp[5] = *cp++;
	dp[4] = *cp++;
	dp[3] = *cp++;
	dp[2] = *cp++;
	dp[1] = *cp++;
	dp[0] = *cp++;

	return ret;
}

static void put_double(char *cp, double fval) {
	unsigned char *dp = (unsigned char *)&fval;

	cp[7] = *dp++;
	cp[6] = *dp++;
	cp[5] = *dp++;
	cp[4] = *dp++;
	cp[3] = *dp++;
	cp[2] = *dp++;
	cp[1] = *dp++;
	cp[0] = *dp++;
}

static void copy_fill(char *dp, const char *sp, int len) {
	while (*sp && len > 0) {
		*dp++ = *sp++;
		len--;
	}
	while (len-- > 0)
		*dp++ = ' ';
}

static void copy_crimp(char *dp, const char *sp, int len) {
	while (len-- > 0) {
		*dp++ = *sp++;
	}
	*dp = 0;
	for (dp-- ; *dp == ' '; dp--) {
		*dp = 0;
	}
}

static void db_set_date(char *cp, int year, int month, int day) {
	if (month > 12)
		month = 0;
	if (day > 31)
		day = 0;
	snprintf(cp, 9, "%04d%02d%02d", year, month, day);
}

static int db_date_year(const char *cp) {
	int	year, i;

	for (year = 0, i = 0; i < 4; i++)
		year = year * 10 + (cp[i] - '0');
	return year;
}

static int db_date_month(const char *cp) {
	int	month, i;

	for (month = 0, i = 4; i < 6; i++)
		month = month * 10 + (cp[i] - '0');
	return month;
}

static int db_date_day(const char *cp) {
	int	day, i;

	for (day = 0, i = 6; i < 8; i++)
		day = day * 10 + (cp[i] - '0');
	return day;
}

static char *db_cur_date(char *cp) {
	struct tm *ctm;
	time_t	  c_time;

	c_time = time((time_t *)NULL);
	ctm = localtime(&c_time);
	//ctm = localtime(&c_time, &tmbuf);
	if (cp == NULL)
		cp = (char *)malloc(9);

	if (ctm == NULL || cp == NULL)
		return NULL;

	db_set_date(cp, ctm->tm_year + 1900, ctm->tm_mon + 1, ctm->tm_mday);

	return cp;
}

static size_t strlcpy(char *dst, const char *src, size_t siz) {
  char *d = dst;
  const char *s = src;
  size_t n = siz;

  /* Copy as many bytes as will fit */
  if (n != 0 && --n != 0) {
    do {
      if ((*d++ = *s++) == 0)
        break;
    } while (--n != 0);
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (siz != 0)
      *d = '\0';                /* NUL-terminate dst */
    while (*s++)
      ;
  }

  return(s - src - 1);        /* count does not include NUL */
}

