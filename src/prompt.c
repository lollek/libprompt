#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "terminal.h"
#include "action.h"
#include "history.h"
#include "cutpaste.h"

#include "prompt.h"

#define ESCAPE 27
#define BACKSPACE 127

char *
prompt(const char *prompt)
{
  return prompt_r(prompt, NULL);
}

char *
prompt_r(const char *prompt, char *buffer)
{
  struct termios oldterm;
  struct termios tmpterm;
  terminal_t terminal = { {'\0'}, 0, 0 };
  int ch;

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
      handle_printables(ch, &terminal);

    else if (ch == ESCAPE)
    {
      ch = getchar();
      switch (ch)
      {
        case 'b': backward_word(&terminal); break;
        case 'f': forward_word(&terminal); break;
        case 'd': kill_word(&terminal); break;
        case '<': history_first_cmd(&terminal); break;
        case '>': history_last_cmd(&terminal); break;
        case '[': /* ARROW KEYS */
          ch = getchar();
          switch (ch)
          {
            case 'A': history_prev_cmd(&terminal); break;
            case 'B': history_next_cmd(&terminal); break;
            case 'C': forward_char(&terminal); break;
            case 'D': backward_char(&terminal); break;
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
      case BACKSPACE: backward_delete_char(&terminal); break;
      case CTRL('A'): beginning_of_line(&terminal); break;
      case CTRL('B'): backward_char(&terminal); break;
      case CTRL('D'): if (terminal.buflen == 0)
                        ch = EOF;
                      else
                        delete_char(&terminal);
                      break;
      case CTRL('E'): end_of_line(&terminal); break;
      case CTRL('F'): forward_char(&terminal); break;
      case CTRL('K'): kill_line(&terminal); break;
      case CTRL('L'): clear_screen(&terminal, prompt); break;
      case CTRL('N'): history_next_cmd(&terminal); break;
      case CTRL('P'): history_prev_cmd(&terminal); break;
      case CTRL('Y'): yank(&terminal); break;
      case CTRL('U'): backward_kill_line(&terminal); break;
#ifdef DEBUG
      default: printf("%d", ch); break;
#endif
    }
  }
  while (ch != EOF);
  putchar('\n');

  tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

  if (ch == EOF && terminal.buflen == 0)
    return NULL;

  if (buffer == NULL)
  {
    buffer = malloc(terminal.buflen + 1);
    if (buffer == NULL)
      return NULL;
  }

  memcpy(buffer, terminal.buf, terminal.buflen);
  buffer[terminal.buflen] = '\0';

  if (buffer[0] != '\0')
    history_save(buffer);

  return buffer;
}

void
prompt_free(void)
{
  history_clear();
  kill_clear();
}
