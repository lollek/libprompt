#include <string.h>
#include <assert.h>

#include "../src/terminal.h"
#include "../src/action.h"

void test_handle_printables(void)
{
  terminal_t term = { {'\0'}, 0, 0 };
  memset(&term, '\0', BUFSIZE);

  /* Test print 1 char on empty term */
  handle_printables('a', &term);
  assert(term.buf[0] == 'a');
  assert(term.buf[1] == '\0');
  assert(term.buflen == 1);
  assert(term.cursorpos == 1);

  /* Test print (append) 1 char on term with only 1 char in it */
  handle_printables('b', &term);
  assert(term.buf[0] == 'a');
  assert(term.buf[1] == 'b');
  assert(term.buf[2] == '\0');
  assert(term.buflen == 2);
  assert(term.cursorpos == 2);

  /* Test print at beginning of buffer */
  term.cursorpos = 0;
  handle_printables('c', &term);
  assert(term.buf[0] == 'c');
  assert(term.buf[1] == 'a');
  assert(term.buf[2] == 'b');
  assert(term.buf[3] == '\0');
  assert(term.buflen == 3);
  assert(term.cursorpos == 1);

  /* Test print at the middle of buffer */
  handle_printables('d', &term);
  assert(term.buf[0] == 'c');
  assert(term.buf[1] == 'd');
  assert(term.buf[2] == 'a');
  assert(term.buf[3] == 'b');
  assert(term.buf[4] == '\0');
  assert(term.buflen == 4);
  assert(term.cursorpos == 2);

  /* Max out the buffer and try to print at the beginning of it */
  term.buflen = BUFSIZE;
  term.cursorpos = 0;
  handle_printables('1', &term);
  assert(term.buf[0] == 'c');
  assert(term.buf[1] == 'd');
  assert(term.buf[2] == 'a');
  assert(term.buf[3] == 'b');
  assert(term.buf[4] == '\0');
  assert(term.buflen == BUFSIZE);
  assert(term.cursorpos == 0);

  /* Try the same in the middle */
  handle_printables('1', &term);
  assert(term.buf[0] == 'c');
  assert(term.buf[1] == 'd');
  assert(term.buf[2] == 'a');
  assert(term.buf[3] == 'b');
  assert(term.buf[4] == '\0');
  assert(term.buflen == BUFSIZE);
  assert(term.cursorpos == 0);

  /* And again at the end */
  handle_printables('1', &term);
  assert(term.buf[0] == 'c');
  assert(term.buf[1] == 'd');
  assert(term.buf[2] == 'a');
  assert(term.buf[3] == 'b');
  assert(term.buf[4] == '\0');
  assert(term.buflen == BUFSIZE);
  assert(term.cursorpos == 0);
}

int main(void)
{
  test_handle_printables();
  return 0;
}
