#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../src/terminal.h"
#include "../src/action.h"
#include "../src/cutpaste.h"

void test_prints(terminal_t *term)
{
  char buf[BUFSIZE +1];
  size_t status;
  printf("[prompt]");

  /* Test print 1 char on empty term */
  status = prints("a", term);
  assert(status == 1);
  assert(term->buf[0] == 'a');
  assert(term->buf[1] == '\0');
  assert(term->buflen == 1);
  assert(term->cursorpos == 1);

  /* Test print (append) 1 char on term with only 1 char in it */
  status = prints("b", term);
  assert(status == 1);
  assert(term->buf[0] == 'a');
  assert(term->buf[1] == 'b');
  assert(term->buf[2] == '\0');
  assert(term->buflen == 2);
  assert(term->cursorpos == 2);

  /* Test print at beginning of buffer */
  beginning_of_line(term);
  assert(term->cursorpos == 0);

  status = prints("c", term);
  assert(status == 1);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'a');
  assert(term->buf[2] == 'b');
  assert(term->buf[3] == '\0');
  assert(term->buflen == 3);
  assert(term->cursorpos == 1);

  /* Test print at the middle of buffer */
  status = prints("d", term);
  assert(status == 1);
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

  memset(buf, '0', sizeof(*buf) * BUFSIZE);
  buf[BUFSIZE] = '\0';
  status = prints(buf, term);
  assert(status == BUFSIZE - 4);
  beginning_of_line(term);

  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 0);

  status = prints("10000000", term);
  assert(status == 0);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 0);

  /* Remove 1 char and try again */
  delete_char(term);
  status = prints("c000000", term);
  assert(status == 1);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 1);

  /* Try the same in the middle */
  status = prints("1", term);
  assert(status == 0);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 1);

  /* Remove 1 char and try again */
  delete_char(term);
  status = prints("d000000", term);
  assert(status == 1);
  assert(term->buf[0] == 'c');
  assert(term->buf[1] == 'd');
  assert(term->buf[2] == 'a');
  assert(term->buf[3] == 'b');
  assert(term->buf[4] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == 2);

  /* And again at the end */
  end_of_line(term);
  assert(term->cursorpos == BUFSIZE);
  assert(term->buflen == BUFSIZE);

  status = prints("1", term);
  assert(status == 0);
  assert(term->buf[BUFSIZE -1] == '0');
  assert(term->buf[BUFSIZE -2] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == BUFSIZE);

  /* Remove 1 char and try again */
  backward_delete_char(term);
  status = prints("1000000", term);
  assert(status == 1);
  assert(term->buf[BUFSIZE -1] == '1');
  assert(term->buf[BUFSIZE -2] == '0');
  assert(term->buflen == BUFSIZE);
  assert(term->cursorpos == BUFSIZE);
}

int main(void)
{
  terminal_t term = { {'\0'}, 0, 0 };
  memset(&term, '\0', BUFSIZE);

  test_prints(&term);

  beginning_of_line(&term);
  kill_line(&term);
  putchar('\n');

  return 0;
}
