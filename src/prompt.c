#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "action.h"

#include "prompt.h"

#define BACKSPACE 127

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

    else if (ch == 27) /* 033 / ESCAPE */
    {
      ch = getchar();
      switch (ch)
      {
        case 'b': backward_word(buf, &chpos); break;
        case 'f': forward_word(buf, &chcounter, &chpos); break;
        case '[': /* ARROW KEYS */
          ch = getchar();
          switch (ch)
          {
            case 'C': forward_char(buf, &chcounter, &chpos); break;
            case 'D': backward_char(&chpos); break;
            default: putchar('\a'); break;
          }
          break;
#ifdef DEBUG
        default: printf("(E)%d", ch); break;
#endif
      }
    }

    else switch (ch)
    {
      case BACKSPACE: handle_backspace(buf, &chcounter, &chpos); break;
      case CTRL('A'): beginning_of_line(&chpos); break;
      case CTRL('B'): backward_char(&chpos); break;
      case CTRL('E'): end_of_line(buf, &chcounter, &chpos); break;
      case CTRL('F'): forward_char(buf, &chcounter, &chpos); break;
      case CTRL('L'): clear_screen(buf, &chcounter, &chpos, prompt); break;
#ifdef DEBUG
      default: printf("%d", ch); break;
#endif
    }
  }
  while (ch != EOF);
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
