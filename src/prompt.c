#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

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
  wchar_t buf[PROMPT_BUF_SIZE];
  size_t bufsiz;
  char *retval = NULL;

  prompt_r(prompt, buf);
  bufsiz = wcstombs(NULL, buf, 0) +1;
  if (bufsiz == (size_t)-1)
    return NULL;

  retval = malloc(bufsiz);
  wcstombs(retval, buf, bufsiz);
  return retval;
}

wchar_t *
prompt_r(const char *prompt, wchar_t *buffer)
{
  struct termios oldterm;
  struct termios tmpterm;
  terminal_t terminal = { {'\0'}, 0, 0 };
  wint_t ch;

  tcgetattr(STDIN_FILENO, &oldterm);
  tmpterm = oldterm;
  tmpterm.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tmpterm);

  if (prompt != NULL)
    printf("%s", prompt);

  do
  {
    ch = getwchar();

    if (ch == WEOF || ch == L'\n')
      break;

    else if (iswprint(ch))
      printc(ch, &terminal);

    else if (ch == ESCAPE)
    {
      ch = getwchar();
      switch (ch)
      {
        case 'b': backward_word(&terminal); break;
        case 'f': forward_word(&terminal); break;
        case 'd': kill_word(&terminal); break;
        case '<': history_first_cmd(&terminal); break;
        case '>': history_last_cmd(&terminal); break;
        case '[': /* ARROW KEYS */
          ch = getwchar();
          switch (ch)
          {
            case 'A': history_prev_cmd(&terminal); break;
            case 'B': history_next_cmd(&terminal); break;
            case 'C': forward_char(&terminal); break;
            case 'D': backward_char(&terminal); break;
            default: putwchar(L'\a'); break;
          }
          break;
#ifdef DEBUG
        default: wprintf(L"(E)%d", ch); break;
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
      default: wprintf(L"%d", ch); break;
#endif
    }
  }
  while (ch != WEOF);
  putwchar(L'\n');

  tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

  if (ch == WEOF && terminal.buflen == 0)
    return NULL;

  if (buffer == NULL)
  {
    buffer = malloc(sizeof(wchar_t) * (terminal.buflen + 1));
    if (buffer == NULL)
      return NULL;
  }

  wmemcpy(buffer, terminal.buf, terminal.buflen);
  buffer[terminal.buflen] = L'\0';

  if (buffer[0] != L'\0')
    history_save(buffer);

  return buffer;
}

void
prompt_free(void)
{
  history_clear();
  kill_clear();
}
