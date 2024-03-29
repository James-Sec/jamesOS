#include <string.h>


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
