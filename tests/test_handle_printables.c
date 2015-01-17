#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../src/terminal.h"
#include "../src/action.h"
#include "../src/cutpaste.h"

void test_handle_printables(terminal_t *term)
{
  printf("[prompt]");

  /* Test print 1 char on empty term */
  handle_printables('a', term);
  assert(term->buf[0] == 'a');
  assert(term->buf[1] == '\0');
  assert(term->buflen == 1);
  assert(term->cursorpos == 1);

  /* Test print (append) 1 char on term with only 1 char in it */
  handle_printables('b', term);
  assert(term->buf[0] == 'a');
  assert(term->buf[1] == 'b');
  assert(term->buf[2] == '\0');
  assert(term->buflen == 2);
  assert(term->cursorpos == 2);

  /* Test print at beginning of buffer */
  beginning_of_line(term);
  assert(term->cursorpos == 0);

  handle_printables('c', term);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'a');
  assert(term->buf[2] == 'b');
  assert(term->buf[3] == '\0');
  assert(term->buflen == 3);
  assert(term->cursorpos == 1);

  /* Test print at the middle of buffer */
  handle_printables('d', term);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '\0');
  assert(term->buflen == 4);
  assert(term->cursorpos == 2);

  /* Max out the buffer and try to print at the beginning of it */
  forward_char(term);
  forward_char(term);
  assert(term->cursorpos == term->buflen);

  while (term->buflen < BUFSIZE)
    handle_printables('0', term);
  beginning_of_line(term);

  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 0);

  handle_printables('1', term);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 0);

  /* Try the same in the middle */
  forward_char(term);
  assert(term->cursorpos == 1);
  assert(term->buflen == BUFSIZE);

  handle_printables('1', term);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 1);

  /* And again at the end */
  end_of_line(term);
  assert(term->cursorpos == BUFSIZE);
  assert(term->buflen == BUFSIZE);

  handle_printables('1', term);
  assert(term->buf[BUFSIZE -1] == '0');
  assert(term->buf[BUFSIZE -2] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == BUFSIZE);
}

int main(void)
{
  terminal_t term = { {'\0'}, 0, 0 };
  memset(&term, '\0', BUFSIZE);

  test_handle_printables(&term);

  beginning_of_line(&term);
  kill_line(&term);

  return 0;
}
