#ifndef PROMPT_CUTPASTE_H
#define PROMPT_CUTPASTE_H

#include "terminal.h"

void
kill_line(terminal_t *term);

void
kill_clear(void);

void
yank(char buf[], unsigned *counter, unsigned *pos);

void
backward_kill_line(terminal_t *term);

void
kill_word(terminal_t *term);

#endif /* PROMPT_CUTPASTE_H */
