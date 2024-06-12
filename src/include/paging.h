#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <isr.h>
#include <kheap.h>

#define INDEX_FROM_BIT(a) (a/(8))
#define OFFSET_FROM_BIT(a) (a%(8))

typedef struct page_dir_entry { 
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t w_through  : 1;
    uint32_t cache      : 1;
    uint32_t accessed   : 1;
    uint32_t reserved   : 1;
    uint32_t page_size  : 1;
    uint32_t ignored    : 1;
    uint32_t available  : 3;
    uint32_t frame      : 20;
}page_dir_entry_t;

typedef struct page_table_entry { 
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t reserved   : 2;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t reserved2  : 2;
    uint32_t available  : 3;
    uint32_t frame      : 20;
} page_table_entry_t;

typedef struct page_table 
{
  page_table_entry_t pages [1024];
} page_table_t;

typedef struct page_directory 
{
  page_dir_entry_t tables_physical [1024];
  page_table_t *tables [1024];
}page_directory_t;

//global functions
void paging_init ();
void switch_page_directory (page_directory_t *dir, uint8_t phy);
page_table_entry_t* get_page (uint32_t address, int make, page_directory_t *dir);
uint32_t virtual2phys (page_directory_t* dir, uint32_t virtual_addr);

//global vars
page_directory_t *kernel_directory=0;
page_directory_t *current_directory=0;

#endif
