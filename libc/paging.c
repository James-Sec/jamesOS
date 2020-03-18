#include "../include/paging.h"
page_directory_t *kernel_direcoty = 0;
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
  asm ("mov %0, %%cr3" :: "r"(dir->tables_physical));
  uint32_t cr0;
  asm ("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;
  asm ("mov %0, %%cr0":: "r"(cr0));
}

void page_init ()
{

  kernel_directory = (page_directory_t*) kmalloc ((sizeof(page_directory_t) & 0xfffff000)+ 0x1000);
  current_directory = kernel_directory;
  //TODO CONTINUE... -> https://github.com/cirosantilli/jamesmolloy-kernel-development-tutorials/blob/master/6_paging/paging.c
}


void page_fault (registers_t *regs)
{
  kprint ("FECHEM OS PORTOOOOOES!!!!!\nESTAMOS SENDO ATACAAAAAADOS!!!!\n");
  while(2);
}
