#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "action.h"

void
handle_printables(int ch, char buf[], unsigned *counter, unsigned *pos)
{
  if (*counter == BUFSIZE)
    putchar('\a');
  else if (*pos != *counter)
  {
    unsigned i;
    memmove(buf + *pos +1, buf + *pos, *counter - *pos);
    buf[*pos] = ch;
    (*counter)++;

    buf[*counter] = '\0';
    printf("%s", buf + *pos);
    (*pos)++;

    for (i = *pos; i < *counter; ++i)
      putchar('\b');
  }
  else
  {
    putchar(ch);
    buf[*pos] = ch;
    (*counter)++;
    (*pos)++;
  }
}

void
backward_delete_char(char buf[], unsigned *counter, unsigned *pos)
{
  if (*pos == 0)
    putchar('\a');
  else if (*pos == *counter)
  {
    printf("\b \b");
    (*counter)--;
    (*pos)--;
  }
  else
  {
    unsigned i;
    memmove(buf + *pos -1, buf + *pos, *counter - *pos);
    (*counter)--;
    (*pos)--;

    buf[*counter] = '\0';
    printf("\b%s \b", buf + *pos);
    for (i = *pos; i < *counter; ++i)
      putchar('\b');
  }
}

void
backward_word(char buf[], unsigned *pos)
{
  if (*pos == 0)
    return;
  do
    backward_char(pos);
  while (*pos > 0 && (!isalnum(buf[*pos]) || isalnum(buf[*pos -1])));
}

void
beginning_of_line(unsigned *pos)
{
  while (*pos > 0)
    backward_char(pos);
}

void
backward_char(unsigned *pos)
{
  if (*pos == 0)
    putchar('\a');
  else
  {
    printf("\b");
    (*pos)--;
  }
}

void
clear_screen(char buf[], unsigned *counter, unsigned *pos, const char *prompt)
{
  printf("\033[2J\033[;H");
  if (prompt != NULL)
    printf("%s", prompt);
  if (*counter != 0)
  {
    unsigned i = 0;
    for (i = 0; i < *counter; ++i)
      putchar(buf[i]);
    for (i = *counter; i > *pos; --i)
      putchar('\b');
  }
}

void
clear_prompt(unsigned *pos)
{
  beginning_of_line(pos);
  printf("\033[K\033[J");
}

void
delete_char(char buf[], unsigned *counter, unsigned *pos)
{
  if (forward_char(buf, counter, pos) == 0)
    backward_delete_char(buf, counter, pos);
}

void
end_of_line(char buf[], unsigned *counter, unsigned *pos)
{
  while (*pos < *counter)
    forward_char(buf, counter, pos);
}

int
forward_char(char buf[], unsigned *counter, unsigned *pos)
{
  if (*pos < *counter)
  {
    putchar(buf[*pos]);
    (*pos)++;
    return 0;
  }
  else
  {
    putchar('\a');
    return 1;
  }
}

void
forward_word(char buf[], unsigned *counter, unsigned *pos)
{
  while (*pos < *counter)
  {
    forward_char(buf, counter, pos);
    if (!isalnum(buf[*pos]) && isalnum(buf[*pos -1]))
      return;
  }
}

void
prompt_set_text(char newdata[], char buf[], unsigned *counter, unsigned *pos)
{
  const size_t newdatasiz = strlen(newdata);

  clear_prompt(pos);

  strcpy(buf, newdata);
  printf("%s", buf);

  *pos = *counter = newdatasiz;
}
