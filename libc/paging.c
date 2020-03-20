#include "../include/paging.h"

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

void alloc_frame (page_t *page, int is_kernel, int is_writable)
{
  if (page->frame)
    return;
  else
  {
    char s [10];
    uint32_t idx = (uint32_t)kmalloc (0x1000);
    if (!idx)
      kprint ("No free frames\n");
    else
    { 
      page->present = 1;
      page->rw = is_writable;
      page->user = is_kernel;
      page->frame = idx >> 12;
    }
  }
}
void free_frame (page_t *page)
{
  if(!page->present)
    return;
  page->present=0;
  kfree (0x1000, (uint32_t) page->frame << 12);
} 

void switch_page_directory (page_directory_t *dir)
{
  current_directory = dir;
  asm volatile("mov %0, %%cr3":: "r"(&dir->tables_physical));
  /*
  uint32_t cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
  */
}

void page_init ()
{
  kernel_directory = (page_directory_t*) kmalloc ((sizeof(page_directory_t) & 0xfffff000)+ 0x1000);

  
  
  current_directory = kernel_directory;
 
  register_interrupt_handler (14, page_fault);
  switch_page_directory (kernel_directory);
}

page_t* get_page (uint32_t virt_addr, int make, page_directory_t* dir)
{
  /*
  virt_addr /= 0x1000;
  uint32_t table_idx = virt_addr / 1024;
  if (dir->tables [table_idx])
    return &dir->tables [table_idx]->pages [virt_addr % 1024];
  else if (make)
  {
    dir->tables[table_idx] = (page_table_t*) kmalloc (sizeof(page_table_t));
    dir->tables_physical[table_idx] = &dir->tables[table_idx] | 0x7;
    return dir->tables[table_idx]->pages[virt_addr%1024];
  }
  */
  
}

void page_fault (registers_t *regs)
{
  kprint ("FECHEM OS PORTOOOOOES!!!!!\nESTAMOS SENDO ATACAAAAAADOS!!!!\n");
  while(2);
}
