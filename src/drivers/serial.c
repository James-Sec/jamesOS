#include <serial.h>

void serial_init ()
{
  // disabling interrupts
  port_byte_out (COM1 + 1, 0x00);
  // setting DLAB to 1
  port_byte_out (COM1 + 3, 0x80);
  // setting the baud rate divisor
  port_byte_out (COM1 + 0, 0x01);
  port_byte_out (COM1 + 1, 0x00);
  // setting character size to 8bits and DLAB to 0
  port_byte_out (COM1 + 3, 0x03);
  // enabling serial port interrupt output
  // setting the device to ready to receive
  // setting the device to want to send state
  port_byte_out (COM1 + 4, 0x0a);
}

void serial_send_byte (uint8_t ch)
{
  while (!(port_byte_in (COM1 + 5) & 0b0100000));
  port_byte_out (COM1 + 0, ch);
}

void serial_send_string (uint8_t* msg)
{
  for (uint32_t pos = 0; msg [pos] != 0; pos++)
    serial_send_byte (msg [pos]);
}

void serial_print_hex (uint32_t n)
{
  char hex_v[12] = "0x????????\n\0";
  char map [16] = "0123456789abcdef";
  int int_v = n;
  int tmp = int_v;
  int digit = 2;
  int shift = 28;
  for (;digit < 10; digit++)
  {
    tmp >>= shift;
    tmp &= 0xf;
    hex_v[digit] = map[tmp];
    shift -= 4;
    tmp = int_v;
  }
  serial_send_string (hex_v);
}
