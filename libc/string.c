#include <string.h>

void htoa (uint32_t n, char s[])
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

// K&R implementation
void itoa (uint32_t n, char s[]) 
{
  uint32_t i, sign;
  if ((sign = n) < 0) 
    n = -n;
  i = 0;
  do
  {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';

  reverse(s);
}

// K&R
void reverse(char s[]) 
{
  int c, i, j;
  for (i = 0, j = strlen(s)-1; i < j; i++, j--)
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

// K&R
int strlen (char s[]) 
{
  int i = 0;
  while (s[i] != '\0')
    ++i;
  return i;
}
