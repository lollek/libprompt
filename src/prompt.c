#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "prompt.h"

char *prompt(const char *prompt)
{
  struct termios oldterm;
  struct termios tmpterm;
  char buf[BUFSIZ];
  char *retval = NULL;
  int ch;
  size_t chcounter = 0;
  size_t chpos = chcounter;


  tcgetattr(STDIN_FILENO, &oldterm);
  tmpterm = oldterm;
  tmpterm.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tmpterm);

  if (prompt != NULL)
    printf("%s", prompt);

  do
  {
    ch = getchar();
    if (ch == EOF || ch == '\n')
      break;

    else if (isprint(ch))
    {
      putchar(ch);
      buf[chpos] = ch;
      if (chpos == chcounter)
        chcounter++;
      chpos++;
    }

    else if (ch == CTRL('A') && chpos > 0)
    {
      printf("\033[%dD", chpos);
      chpos = 0;
    }

    else if (ch == CTRL('B'))
    {
      if (chpos > 0)
      {
        printf("\033[1D");
        chpos--;
      }
      else
        putchar('\a');
    }

    else if (ch == CTRL('E') && chpos < chcounter)
    {
      printf("\033[%dC", chcounter - chpos);
      chpos = chcounter;
    }
  }
  while (ch != EOF && chcounter < BUFSIZ);
  putchar('\n');

  tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

  if (ch == EOF && chcounter == 0)
    return NULL;

  retval = malloc(chcounter + 1);
  if (retval == NULL)
    return NULL;

  memcpy(retval, buf, chcounter);
  retval[chcounter] = '\0';
  return retval;
}
