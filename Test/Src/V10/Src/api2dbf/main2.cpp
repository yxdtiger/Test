#include <iostream>
#include <fcntl.h>
#include "dbf/dbf.h"
#include "../include/wmdf_api.h"
#include "../include/wmdf_structs.h"

int main2(int argc, char* argv[])
{
  char* file = "D:\\fxindex.dbf";
  dbhead_t* dbh = dbf_open(file,O_RDONLY);
  //dbf_head_info(dbh);
  IndexSnapshot* mktItems = new IndexSnapshot[dbh->db_records];
  memset(mktItems,0,sizeof(IndexSnapshot)*dbh->db_records);
  printf("windcode\t pre_close\t open\t high\t low\t new\n");
  while(dbf_next(dbh))
  {
    printf("%s\t ",dbf_get_string(dbh,0,NULL,6));
    printf("%.2f\t ",dbf_get_num(dbh,2));
    printf("%.2f\t ",dbf_get_num(dbh,3));
    printf("%.2f\t ",dbf_get_num(dbh,5));
    printf("%.2f\t ",dbf_get_num(dbh,6));
    printf("%.2f \n",dbf_get_num(dbh,7));
  }

  dbf_destroy(dbh);
  getchar();
  return NULL;
}

//int main() {
//  dbhead_t *dbh = dbf_new("./test.dbf", "name:C,10;price:N,10,4;");
//
//  dbf_append(dbh, 20000);
//  while(dbf_next(dbh)) {
//    dbf_set_string(dbh, 0, "hello");
//    dbf_set_num(dbh, 1, 100);
//  }
//  dbf_flush(dbh);
//
//  dbf_destroy(dbh);
//
//  dbh = dbf_open("./test.dbf", O_RDONLY);
//  if (dbh == NULL) {
//    printf("open failed");
//    exit(1);
//  }
//  dbf_reset(dbh);
//  while(dbf_next(dbh))
//    dbf_print(dbh);
//  dbf_destroy(dbh);
//
//  return EXIT_SUCCESS;
//}