#include "../include/vga.h"

// auxiliary functions
static uint32_t print_char (uint8_t character, uint8_t attributeint, uint32_t offset);
static uint32_t get_cursor_offset();
static void set_cursor_offset (uint32_t offset);
static void scroll_screen();
  
// default character color
uint8_t attribute = YELLOW_ON_BLACK; 

static uint32_t print_char (uint8_t character, uint8_t attribute, uint32_t offset) 
{
  if(character == '\n')
  {
    offset /= 2;
    uint32_t row = offset / MAX_COLS;
    row++;
    return MAX_COLS * row * 2;
  }
  uint8_t *vid_addr = (uint8_t*) VIDEO_ADDRESS;
  *(vid_addr + offset) = character;
  offset++;
  *(vid_addr + offset) = attribute;
  offset++;
  return offset;
}

static uint32_t get_cursor_offset ()
{
  uint32_t offset;
  port_byte_out (VGA_CTRL_PORT,14);
  offset = port_byte_in (VGA_DATA_PORT);
  offset <<= 8;
  port_byte_out (VGA_CTRL_PORT, 15);
  offset += port_byte_in (VGA_DATA_PORT);
  return offset;
}

static void set_cursor_offset (uint32_t offset)
{
  uint8_t offset_byte = (uint8_t) (offset>>8);
  port_byte_out (VGA_CTRL_PORT, 14);
  port_byte_out (VGA_DATA_PORT, offset_byte);
  port_byte_out (VGA_CTRL_PORT, 15);
  offset_byte = (uint8_t) (offset & 0xff);
  port_byte_out (VGA_DATA_PORT, offset_byte);
}

static void scroll_screen ()
{
  uint8_t *vid_addr = (uint8_t *) VIDEO_ADDRESS;
  vid_addr = vid_addr + (MAX_COLS * 2); 
  uint32_t i;
  for(i = 0 ; i < MAX_ROWS; i++)
  {
    memcpy (vid_addr, vid_addr - (MAX_COLS * 2) , MAX_COLS*2);
    vid_addr = vid_addr + (MAX_COLS * 2) ;
  }
  set_cursor_offset (MAX_ROWS * MAX_COLS - MAX_COLS);
}

void kprint_at (uint8_t *message, int32_t offset)
{
  if(offset < 0)
    offset = get_cursor_offset () * 2;
  for (;*message != '\0' ; message ++)
  {
    offset = print_char(*message, attribute, offset);
    if (offset > MAX_COLS * MAX_ROWS * 2 - 1)
    {
      scroll_screen();
      offset = get_cursor_offset() * 2;
    }
  }
  set_cursor_offset(offset / 2);
}

void kprint (uint8_t *message){
  kprint_at (message, -1);
}

void kprint_debug (uint8_t *message, uint8_t color)
{
  attribute = color;
  kprint_at (message, -1);
  attribute = YELLOW_ON_BLACK;
}

void kprintf (char *str, int n, ...)
{
  char buff [strlen (str)];
  va_list ap;
  va_start (ap, n);
  int i, index = 0;
  for (i = 0; i < strlen (str); i++)
  {
    if (str[i] == '%')
    {
      if (str[i+1] != '%')
      {
        buff[index] = '\0';
        kprint (buff);
        if (str[i+1] == 'd')
        {
          char s[10];
          itoa (va_arg (ap, int), s);
          kprint (s);
        }
        else if (str[i+1] == 's')
          kprint (va_arg (ap, char*));

        index = 0;
        ++i;
      }
      else
      {
        buff[index] = str[i];
        ++index;
        ++i;
      }
    }
    else
    {
      buff[index] = str[i];
      ++index;
    }
  }
  if (index > 0)
  {
    buff[index] = '\0';
    kprint (buff);
  }
}

void clear_screen()
{
  uint32_t i;
  for (i = 0; i < MAX_ROWS * MAX_COLS * 2; i+=2)
    print_char(' ', WHITE_ON_BLACK, i);
  set_cursor_offset (0);
}
