#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "prompt.h"

#define DEBUG
#define BACKSPACE 127
#define BUFSIZE 4096

char *
prompt(const char *prompt)
{
  struct termios oldterm;
  struct termios tmpterm;
  char buf[BUFSIZE];
  char *retval = NULL;
  int ch;
  unsigned chcounter = 0;
  unsigned chpos = chcounter;


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
      printf("\033[%uD", chpos);
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

    else if (ch == CTRL('D') && chcounter == 0)
    {
      ch = EOF;
      break;
    }

    else if (ch == CTRL('E') && chpos < chcounter)
    {
      printf("\033[%dC", chcounter - chpos);
      chpos = chcounter;
    }

    else if (ch == CTRL('F'))
    {
      if (chpos < chcounter)
      {
        printf("\033[1C");
        chpos++;
      }
      else
        putchar('\a');
    }

    else if (ch == BACKSPACE)
    {
      if (chpos > 0 && chpos == chcounter)
      {
        printf("\033[1D \033[1D");
        chcounter--;
        chpos--;
      }
      else
        putchar('\a');
    }

#ifdef DEBUG
    else
      printf("%d\n", ch);
#endif
  }
  while (ch != EOF && chcounter < BUFSIZE);
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
