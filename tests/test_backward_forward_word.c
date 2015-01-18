#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../src/terminal.h"
#include "../src/action.h"
#include "../src/cutpaste.h"

void test_backward_forward_word(terminal_t *term)
{
  const char *tststr = "  ! Hello World  Hello  World---Hello   World&&Hello";
  const size_t tstlen = strlen(tststr);

  /* Set up */
  memcpy(term->buf, tststr, tstlen);
  term->buflen = tstlen;
  term->cursorpos = term->buflen;
  printf("[prompt]%s", term->buf);

  /* Back up to beginning */
  assert(term->buf[term->cursorpos -1] == 'o');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'H');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'W');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'H');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'W');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'H');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'W');

  backward_word(term);
  assert(term->buf[term->cursorpos] == 'H');

  backward_word(term);
  assert(term->buf[term->cursorpos] == ' ');
  assert(term->buflen == tstlen);
  assert(term->cursorpos == 0);

  backward_word(term);
  assert(term->buf[term->cursorpos] == ' ');
  assert(term->buflen == tstlen);
  assert(term->cursorpos == 0);

  /* Go forward to end */
  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'o');
  assert(term->buf[term->cursorpos] == ' ');
  assert(term->buf[term->cursorpos +1] == 'W');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'd');
  assert(term->buf[term->cursorpos] == ' ');
  assert(term->buf[term->cursorpos +1] == ' ');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'o');
  assert(term->buf[term->cursorpos] == ' ');
  assert(term->buf[term->cursorpos +1] == ' ');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'd');
  assert(term->buf[term->cursorpos] == '-');
  assert(term->buf[term->cursorpos +1] == '-');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'o');
  assert(term->buf[term->cursorpos] == ' ');
  assert(term->buf[term->cursorpos +1] == ' ');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'd');
  assert(term->buf[term->cursorpos] == '&');
  assert(term->buf[term->cursorpos +1] == '&');

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'o');
  assert(term->buflen == tstlen);
  assert(term->cursorpos == tstlen);

  forward_word(term);
  assert(term->buf[term->cursorpos -1] == 'o');
  assert(term->buflen == tstlen);
  assert(term->cursorpos == tstlen);
}

int main(void)
{
  terminal_t term = { {'\0'}, 0, 0 };
  memset(&term, '\0', BUFSIZE);

  test_backward_forward_word(&term);

  beginning_of_line(&term);
  kill_line(&term);

  return 0;
}
