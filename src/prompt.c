#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "action.h"
#include "history.h"
#include "cutpaste.h"

#include "prompt.h"

#define ESCAPE 27
#define BACKSPACE 127
#define trigger_eof() do { ch = EOF; goto break_loop; } while(0)

char *
prompt(const char *prompt)
{
  struct termios oldterm;
  struct termios tmpterm;
  char buf[BUFSIZE +1];
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
      handle_printables(ch, buf, &chcounter, &chpos);

    else if (ch == ESCAPE)
    {
      ch = getchar();
      switch (ch)
      {
        case 'b': backward_word(buf, &chpos); break;
        case 'f': forward_word(buf, &chcounter, &chpos); break;
        case 'd': kill_word(buf, &chcounter, &chpos); break;
        case '<': history_first_cmd(buf, &chcounter, &chpos); break;
        case '>': history_last_cmd(buf, &chcounter, &chpos); break;
        case '[': /* ARROW KEYS */
          ch = getchar();
          switch (ch)
          {
            case 'A': history_prev_cmd(buf, &chcounter, &chpos); break;
            case 'B': history_next_cmd(buf, &chcounter, &chpos); break;
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
      case BACKSPACE: backward_delete_char(buf, &chcounter, &chpos); break;
      case CTRL('A'): beginning_of_line(&chpos); break;
      case CTRL('B'): backward_char(&chpos); break;
      case CTRL('D'): if (chcounter == 0)
                        trigger_eof();
                      else
                        delete_char(buf, &chcounter, &chpos);
                      break;
      case CTRL('E'): end_of_line(buf, &chcounter, &chpos); break;
      case CTRL('F'): forward_char(buf, &chcounter, &chpos); break;
      case CTRL('K'): kill_line(buf, &chcounter, &chpos); break;
      case CTRL('L'): clear_screen(buf, &chcounter, &chpos, prompt); break;
      case CTRL('N'): history_next_cmd(buf, &chcounter, &chpos); break;
      case CTRL('P'): history_prev_cmd(buf, &chcounter, &chpos); break;
      case CTRL('Y'): yank(buf, &chcounter, &chpos); break;
      case CTRL('U'): backward_kill_line(buf, &chcounter, &chpos); break;
#ifdef DEBUG
      default: printf("%d", ch); break;
#endif
    }
  }
  while (ch != EOF);
break_loop:
  putchar('\n');

  tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

  if (ch == EOF && chcounter == 0)
    return NULL;

  retval = malloc(chcounter + 1);
  if (retval == NULL)
    return NULL;

  memcpy(retval, buf, chcounter);
  retval[chcounter] = '\0';

  if (retval[0] != '\0')
    history_save(retval);

  return retval;
}

void
prompt_free(void)
{
  history_clear();
  kill_clear();
}
