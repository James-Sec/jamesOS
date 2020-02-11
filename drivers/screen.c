#include "screen.h"

int print_char (char character, char attributeint, int offset);
void set_cursor_offset (int offset);
int get_cursor_offset();
void scroll_screen();


void kprint (char *message){
  kprint_at (message, -1);
}


/*offset is two bytes by character*/
void kprint_at (char *message, int offset)
{
  if(offset < 0)
    offset = get_cursor_offset () * 2;
  for (;*message != '\0' ; message ++)
  {
    offset = print_char(*message, RED_ON_WHITE, offset);
    if (offset > MAX_COLS * MAX_ROWS * 2 - 1)
    {
      scroll_screen();
      offset = get_cursor_offset() * 2;
    }
  }
  set_cursor_offset(offset / 2);
}


void clear_screen()
{
  int i;
  for (i = 0; i < MAX_ROWS * MAX_COLS * 2; i+=2)
    print_char(' ', WHITE_ON_BLACK, i);
  set_cursor_offset (0);
}


/*offset is two bytes by character*/
int print_char (char character, char attribute, int offset)
{
  if(character == '\n')
  {
    offset /= 2;
    int row = offset / MAX_COLS;
    row++;
    return MAX_COLS * row * 2;
  }
  char *vid_addr = (char*) VIDEO_ADDRESS;
  *(vid_addr + offset) = character;
  offset++;
  *(vid_addr + offset) = attribute;
  offset++;
  return offset;
}


void scroll_screen ()
{
  char *vid_addr = (char *) VIDEO_ADDRESS;
  vid_addr = vid_addr + (MAX_COLS * 2);
  int i;
  for(i = 0 ; i < MAX_ROWS; i++)
  {
    memcpy (vid_addr, vid_addr - (MAX_COLS * 2) , MAX_COLS*2);
    vid_addr = vid_addr + (MAX_COLS * 2) - 1;
  }
  set_cursor_offset (MAX_ROWS * MAX_COLS - MAX_COLS);
}


/*offset is one byte by character*/
void set_cursor_offset (int offset)
{
  char offset_byte = (char) (offset>>8);
  port_byte_out (PORT_SCREEN_CTRL, 14);
  port_byte_out (PORT_SCREEN_DATA, offset_byte);
  port_byte_out (PORT_SCREEN_CTRL, 15);
  offset_byte = (char) (offset & 0xff);
  port_byte_out (PORT_SCREEN_DATA, offset_byte);
}

/*offset is one byte by character*/
int get_cursor_offset ()
{
  int offset;
  port_byte_out (PORT_SCREEN_CTRL,14);
  offset = port_byte_in (PORT_SCREEN_DATA);
  offset <<= 8;
  port_byte_out (PORT_SCREEN_CTRL, 15);
  offset += port_byte_in (PORT_SCREEN_DATA);
  return offset;
}
