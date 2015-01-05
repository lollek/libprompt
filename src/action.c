#include <stdio.h>
#include <ctype.h>

#include "action.h"

void
handle_printables(int ch, char buf[], unsigned *counter, unsigned *pos)
{
  putchar(ch);
  buf[*pos] = ch;
  if (*pos == *counter)
    (*counter)++;
  (*pos)++;
}

void
handle_backspace(unsigned *counter, unsigned *pos)
{
  if (*pos > 0 && *pos == *counter)
  {
    printf("\033[1D \033[1D");
    (*counter)--;
    (*pos)--;
  }
  else
    putchar('\a');
}

void
backward_word(char buf[], unsigned *pos)
{
  while (*pos > 0)
  {
    backward_char(pos);
    if (isalnum(buf[*pos]) && !isalnum(buf[*pos -1]))
      return;
  }
}

void
beginning_of_line(unsigned *pos)
{
  if (*pos == 0)
    return;

  printf("\033[%uD", *pos);
  *pos = 0;
}

void
backward_char(unsigned *pos)
{
  if (*pos == 0)
    putchar('\a');
  else
  {
    printf("\033[1D");
    (*pos)--;
  }
}

void
end_of_line(unsigned *counter, unsigned *pos)
{
  if (*pos < *counter)
  {
    printf("\033[%dC", *counter - *pos);
    *pos = *counter;
  }
}

void
forward_char(unsigned *counter, unsigned *pos)
{
  if (*pos < *counter)
  {
    printf("\033[1C");
    (*pos)++;
  }
  else
    putchar('\a');
}
