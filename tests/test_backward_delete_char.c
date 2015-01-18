#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../src/terminal.h"
#include "../src/action.h"
#include "../src/cutpaste.h"

void test_backward_delete_char(terminal_t *term)
{
  printf("[prompt]");

  handle_printables('h', term);
  handle_printables('i', term);
  assert(term->buf[0] == 'h');
  assert(term->buf[1] == 'i');
  assert(term->buflen == 2);
  assert(term->cursorpos == 2);

  /* Delete last char */
  backward_delete_char(term);
  assert(term->buf[0] == 'h');
  assert(term->buf[1] == 'i');
  assert(term->buflen == 1);
  assert(term->cursorpos == 1);

  /* If cursor is at the start of the line, nothing should happen */
  term->cursorpos = 0;
  backward_delete_char(term);
  assert(term->buf[0] == 'h');
  assert(term->buf[1] == 'i');
  assert(term->buflen == 1);
  assert(term->cursorpos == 0);

  /* Should work when buffer is maxed */
  term->cursorpos = term->buflen;
  while (term->buflen < BUFSIZE -1)
    handle_printables('0', term);
  handle_printables('9', term);

  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == BUFSIZE);
  assert(term->buf[BUFSIZE -2] == '0');
  assert(term->buf[BUFSIZE -1] == '9');

  backward_delete_char(term);
  assert(term->buf[BUFSIZE -2] == '0');
  assert(term->buf[BUFSIZE -1] == '9');
  assert(term->buflen == BUFSIZE -1);
  assert(term->cursorpos == BUFSIZE -1);

  /* Should work from the middle */
  while (term->cursorpos > 2)
    backward_char(term);
  term->buflen -= 4;
  handle_printables('1', term);
  handle_printables('2', term);
  handle_printables('3', term);
  handle_printables('4', term);

  assert(term->buf[2] == '1');
  assert(term->buf[3] == '2');
  assert(term->buf[4] == '3');
  assert(term->buf[5] == '4');
  assert(term->cursorpos == 6);

  backward_char(term);
  backward_char(term);
  backward_char(term);
  backward_char(term);
  assert(term->cursorpos == 2);

  backward_delete_char(term);
  backward_delete_char(term);

  assert(term->buf[0] == '1');
  assert(term->buf[1] == '2');
  assert(term->buf[2] == '3');
  assert(term->buf[3] == '4');
  assert(term->buflen == BUFSIZE -3);
  assert(term->cursorpos == 0);
}

int main(void)
{
  terminal_t term = { {'\0'}, 0, 0 };
  memset(&term, '\0', BUFSIZE);

  test_backward_delete_char(&term);

  kill_line(&term);
  putchar('\n');
  return 0;
}
