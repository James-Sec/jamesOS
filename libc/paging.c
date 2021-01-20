#include <paging.h>

static void enable_paging ();
static void page_fault_handler (registers_t* regs);
static void set_frame (uint32_t frame_addr);
static void clear_frame (uint32_t frame_addr);
static uint32_t first_frame ();
static void alloc_page (page_table_entry_t *page, uint32_t is_kernel, int32_t is_writeable);

uint8_t page_enabled = 0;
uint8_t *bitmap; 
uint32_t nframes;

static void set_frame (uint32_t frame_addr)
{
  uint32_t idx = INDEX_FROM_BIT (frame_addr);
  uint8_t off = OFFSET_FROM_BIT (frame_addr);
  bitmap[idx] |= (0x1 << off);
}

static void clear_frame (uint32_t frame_addr)
{
  uint32_t idx = INDEX_FROM_BIT (frame_addr);
  uint8_t off = OFFSET_FROM_BIT (frame_addr);
  bitmap[idx] &= ~(0x1 << off);
}

static uint32_t first_frame ()
{
  uint32_t i, j;
  for (i = 0; i < INDEX_FROM_BIT (nframes); i++)
    if (bitmap[i] != 0xFF) 
      for (j = 0; j < 8; j++)
        if (!(bitmap[i] & (1 << j)) )
          return i * 8 + j;
  return  -1;
}

static void alloc_page (page_table_entry_t *page, uint32_t is_kernel, int32_t is_writeable)
{
  if (page->present)
    return;
  else
  {
    uint32_t idx = first_frame ();
    if (idx == -1)
    {
      kprint_debug ("Fail in alloc_frame function: no frames available. Halting...", LIGHT_GREEN);
      asm volatile ("cli");
      asm volatile ("hlt");
    }
    set_frame (idx);
    page->present = 1;
    page->rw = is_writeable;
    page->user = (1 + is_kernel) % 2;
    page->frame = idx;
  }
}

void free_page (page_table_entry_t *page)
{
  if (!page->present)
    return;
  else
  {
    clear_frame (page->frame/0x1000);
    page->frame = 0x0;
    page->present = 0x0;
  }
}

void paging_init () {
  uint32_t mem_end_addr = 0x1000000;
  nframes = mem_end_addr / 0x1000;
  bitmap = (uint8_t*) kmalloc (INDEX_FROM_BIT (nframes));
  memset ((uint8_t*) bitmap, 0, INDEX_FROM_BIT (nframes));

  kprintf ("bitmap: %x\n", 1, bitmap);

  kernel_directory = (page_directory_t*) kmalloc (sizeof (page_directory_t));
  current_directory = kernel_directory;

  kprintf ("kernel_directory: %x\n", 1, kernel_directory);

  int i = 0;
  while (i <= HEAP_LIMIT) 
  {
    alloc_page (get_page (i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }
  register_interrupt_handler (14, page_fault_handler);

  enable_paging ();
}

static void enable_paging ()
{
  asm volatile ("mov %0, %%cr3" :: "r" (&kernel_directory->tables_physical));
  uint32_t cr0;
  asm volatile ("mov %%cr0, %0" : "=r" (cr0));
  cr0 |= 0x80000000; 
  asm volatile ("mov %0, %%cr0" :: "r" (cr0));
  page_enabled = 1;
}

void switch_page_directory (page_directory_t *dir, uint8_t phy)
{
  page_directory_t* tmp;
  if (!phy)
    tmp = (page_directory_t*) virtual2phys (current_directory, (uint32_t)dir);
  else
    tmp = dir;
  current_directory = tmp;
  asm volatile ("mov %0, %%cr3" :: "r" (&tmp->tables_physical));
}

page_table_entry_t* get_page (uint32_t address, int make, page_directory_t *dir)
{
  address /= 0x1000;

  uint32_t table_idx = address / 1024;
  if (dir->tables[table_idx]) 
    return &dir->tables[table_idx]->pages[address % 1024];
  else if(make)
  {
    uint32_t tmp;
    dir->tables[table_idx] = (page_table_t*) kmalloc (sizeof (page_table_t));
    tmp = virtual2phys (dir, (uint32_t) dir->tables[table_idx]);
    dir->tables_physical[table_idx].frame = tmp >> 12;
    dir->tables_physical[table_idx].present = 1;
    dir->tables_physical[table_idx].rw = 1;
    dir->tables_physical[table_idx].user = 1;
    dir->tables_physical[table_idx].page_size = 0;
    return &dir->tables[table_idx]->pages[address%1024];
  }
  else
    return 0;
}

uint32_t virtual2phys (page_directory_t *dir, uint32_t virt_addr)
{
  if (!page_enabled)
    return virt_addr;
  page_table_entry_t* page;
  if (!(page = get_page (virt_addr, 0, dir)))
    return 0;
  if (!page->present)
    return 0;
  return ((page->frame << 12) & 0xfff00000) | (virt_addr & 0xfffff);
}

static void page_fault_handler (registers_t* regs)
{
  kprint_debug ("FECHEM OS PORTOOOOOOOOES!! ESTAMOS SENDO ATACAAAAAAAAADOS!!!\n", LIGHT_GREEN);

  uint32_t faulting_addr;
  asm volatile ("mov %%cr2, %0" : "=r" (faulting_addr));
  char str[10];
  itoa (faulting_addr, str);
  kprint_debug ("falting addr: ", LIGHT_GREEN);
  kprint_debug (str, LIGHT_GREEN);
  kprintf (": %x\n", 1, faulting_addr);

  uint8_t present = regs->err_code & 0x1;
  uint8_t rw = regs->err_code & 0x2;
  uint8_t user = regs->err_code & 0x4;

  if (!present) kprint_debug ("page not present\n", LIGHT_GREEN);
  if (rw) kprint_debug ("page is read only\n", LIGHT_GREEN);
  if (user) kprint_debug ("page is read only\n", LIGHT_GREEN);
  // it should not be commented (just for test)
  // kprint_debug ("halting...", LIGHT_GREEN);
  //asm volatile ("cli");
  //asm volatile ("hlt");

  // just for test (it will generate only three page faults)
  static int t = 0; 
  t++;
  while (1);
  if (t == 3) 
    alloc_frame (get_page (faulting_addr, 1, kernel_directory), 0, 0);
}
