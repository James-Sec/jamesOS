#ifndef PAGING_H
#define PAGING_H

#include "types.h"
#include "isr.h"
#include "memory_manager.h"
typedef struct page
{
  uint32_t present  : 1;
  uint32_t rw       : 1;
  uint32_t user     : 1;
  uint32_t accesses : 1;
  uint32_t dirty    : 1;
  uint32_t unused   : 7;
  uint32_t frame    : 20;
}page_t;

typedef struct page_table
{
  page_t pages [1024];
} page_table_t;

typedef struct page_directory
{
  page_table_t *tables [1024];
  uint32_t tables_physical [1024];
  uint32_t physical_addr;
}page_directory_t;

void page_init ();
void switch_page_directory (page_directory_t *dir);
page_t* get_page (uint32_t address, int make, page_directory_t *dir);
void page_fault (registers_t *regs);
#endif
