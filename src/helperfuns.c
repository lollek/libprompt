#include <stdio.h>

#include "helperfuns.h"

void
printbuf(char buf[], unsigned *counter, unsigned *pos)
{
  if (*counter < BUFSIZE -1)
  {
    buf[*counter] = '\0';
    printf("%s", buf + *pos);
  }
  else
  {
    char lastchar = buf[BUFSIZE -1];
    buf[BUFSIZE -1] = '\0';
    printf("%s", buf + *pos);
    buf[BUFSIZE -1] = lastchar;
    putchar(lastchar);
  }
}
