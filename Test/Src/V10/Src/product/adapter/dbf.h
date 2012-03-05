#ifndef	DBF_H_
#define	DBF_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DBF_NAMELEN 11

struct db_field {
	char	db_fname[DBF_NAMELEN+1];	/* 0 terminated */
	char	db_type;		        /* type of field */
	int	db_flen;		          /* length of field */
	int	db_fdc;			          /* number of decimals in field */

	char	*db_format;		      /* format for printing %s etc */
	int	db_foffset;		        /* offset within record */
};
typedef struct db_field	dbfield_t;

//Data Type                Symbol       //Description 
#define DBF_CHARACTER      'C'          //All OEM code page characters - padded with blanks to the width of the field. 
#define DBF_DATE           'D'          //8 bytes - date stored as a string in the format YYYYMMDD. 
#define DBF_NUMERIC        'N'          //Number stored as a string, right justified, and padded with blanks to the width of the field.  
#define DBF_LOGICAL        'L'          //1 byte - initialized to 0x20 (space) otherwise T or F. 
#define DBF_MEMO           'M'          //a string 10 digits (bytes) representing a .DBT block number. The number is stored as a string, right justified and padded with blanks. 
  
struct db_head {
	int	db_fd;
	unsigned char	db_dbt;			/* dbt present */
	char	db_date[9];		      /* date of last update in db format */
	long	db_records;		      /* number of records */
	int	db_hlen;		          /* header length */
	int	db_rlen;		          /* record length */

	int	db_nfields;		        /* number of fields */
	dbfield_t	*db_fields;	    /* field info */
	char	*db_name;		        /* name of dbf file */

  /* workspace */
	int	db_cur_rec;		        /* current record */
  char *db_cur_data;        /* raw data of current record */
  char cache_dirty;         /* cache is dirty  */
  char *cache;
  int cache_size;        /*the capacity of cache */
  int cache_beg;         /*begin rec num in cache */
  int cache_end;         /*end rec num in cache */
};
typedef struct db_head	dbhead_t;

#define	DBH_TYPE_NORMAL	0x03   //dBase III
#define	DBH_TYPE_MEMO	  0x80   //mask for memo field.

#define CACHE_SIZE 1024*8

#define DBF_E_NONE  "no error"
#define DBF_E_READ  "open file failed"
#define DBF_E_WRITE "write file failed"
#define DBF_E_FTYPE "malformed field type"
#define DBF_E_FNAME "malformed field name"
#define DBF_E_FLEN  "malformed field length"
#define DBF_E_FDEC  "malformed field decimal"
#define DBF_E_FILE  "malformed dbf file"
#define DBF_E_NOTSUPPORT  "unsupported field"
#define DBF_E_MEM  "no enough memory"
#define DBF_E_COMMA  "',' expected in schema"
#define DBF_E_SEMICOLON  "';' expected in schema"
#define DBF_E_FDEC_TOOLONG  "decimal too long"
#define DBF_E_MEMO_NOT_SUPPORT  "'memo' not support"
#define DBF_E_OPEN  "file open failed"
#define DBF_E_INVALID_FILE_HANDLE  "invalid file handle"
                              
const char* dbf_error();

dbhead_t *dbf_new(const char *file, const char* field_desc);
dbhead_t *dbf_open(const char *file, int o_flags);
int dbf_truncate(dbhead_t *dbh);
void dbf_destroy(dbhead_t *dbh);
void pack_dbf(dbhead_t *dbh);
void dbf_head_info(dbhead_t *dbh);

int dbf_reset(dbhead_t *dbh);
int dbf_next(dbhead_t *dbh);
//NOTE: dbf_next_record 与 dbf_next的区别为，
//      dbf_next 似照 dbase III标准忽略已删除记录
//      dbf_next_record 不考虑删除标记
int dbf_next_record(dbhead_t *dbh);
int dbf_moveto(dbhead_t *dbh, int rec_num);
void dbf_print(dbhead_t *dbh);

int dbf_append(dbhead_t *dbh, int n);
int dbf_delete(dbhead_t *dbh, int rec_num);
int dbf_flush(dbhead_t *dbh);

size_t dbf_get_val(dbhead_t *dbh, int idx, char *buf, size_t size);
void dbf_set_val(dbhead_t *dbh, int idx, const char *buf);

#ifndef lua_h
#define luai_NI		((int)0x80000000)
#if defined(_WIN32)
#define luai_NL		((long long)0x8000000000000000)
#define luai_NN		(log(-1.0))
#else
#define luai_NL		(0x8000000000000000LL)
#define luai_NN		(0/0.0)
#endif

#define isNaN(n) ((n)!=(n))
#define isNaI(n) ((n)==luai_NI)
#define isNaL(n) ((n)==luai_NL)
#endif

int dbf_get_int(dbhead_t *dbh, int idx);
long long dbf_get_long(dbhead_t *dbh, int idx);
double dbf_get_num(dbhead_t *dbh, int idx);
char *dbf_get_string(dbhead_t *dbh, int idx, char *ret, size_t size);
char dbf_get_logical(dbhead_t *dbh, int idx);
int dbf_get_date(dbhead_t *dbh, int idx);

//该函数与dbf_next_record一起使用，用于提取&设置记录的删除标记
char dbf_get_flag(dbhead_t *dbh);
void dbf_set_flag(dbhead_t *dbh, char flag);

void dbf_set_int(dbhead_t *dbh, int idx, int val);
void dbf_set_long(dbhead_t *dbh, int idx, long long val);
void dbf_set_num(dbhead_t *dbh, int idx, double val);
void dbf_set_string(dbhead_t *dbh, int idx, const char *str);
void dbf_set_logical(dbhead_t *dbh, int idx, char ch);
void dbf_set_date(dbhead_t *dbh, int idx, int yyyymmdd);

#ifdef __cplusplus
}
#endif
#endif	/* DBF_H_ */
