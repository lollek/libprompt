#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../src/terminal.h"
#include "../src/action.h"
#include "../src/cutpaste.h"

/* TODO: Add test for multi-line */

void test_bol(terminal_t *term)
{
  const char *string = "HELLO WORLD";
  const size_t strlen = sizeof("HELLO WORLD");

  printf("[prompt]");
  memcpy(term->buf, string, strlen);
  term->buflen = strlen;
  term->cursorpos = strlen;
  assert(term->buf[term->cursorpos -1] = 'D');

  beginning_of_line(term);
  assert(term->buf[term->cursorpos] = 'H');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] = 'O');
  assert(term->buf[term->cursorpos] = ' ');
  assert(term->buf[term->cursorpos +1] = 'W');

  beginning_of_line(term);
  assert(term->buf[term->cursorpos] = 'H');

  beginning_of_line(term);
  assert(term->buf[term->cursorpos] = 'H');
}

int main(void)
{
  terminal_t term = { {'\0'}, 0, 0 };
  memset(&term, '\0', BUFSIZE);

  test_bol(&term);

  beginning_of_line(&term);
  kill_line(&term);
  putchar('\n');

  return 0;
}
