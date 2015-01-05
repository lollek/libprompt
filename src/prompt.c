#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "action.h"

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

    if (ch == CTRL('D') && chcounter == 0)
      ch = EOF;
    if (ch == EOF || ch == '\n')
      break;

    else if (isprint(ch))
      handle_printables(ch, buf, &chcounter, &chpos);

    else switch (ch)
    {
      case BACKSPACE: handle_backspace(&chcounter, &chpos); break;
      case CTRL('A'): beginning_of_line(&chpos); break;
      case CTRL('B'): backward_char(&chpos); break;
      case CTRL('E'): end_of_line(&chcounter, &chpos); break;
      case CTRL('F'): forward_char(&chcounter, &chpos); break;
#ifdef DEBUG
      default: printf("%d\n", ch); break;
#endif
    }
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
