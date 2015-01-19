#ifndef PROMPT_ACTION_H
#define PROMPT_ACTION_H

#include <wchar.h>

#include "terminal.h"

void
printc(wchar_t ch, terminal_t *term);

void
backward_delete_char(terminal_t *term);

void
backward_word(terminal_t *term);

void
beginning_of_line(terminal_t *term);

void
backward_char(terminal_t *term);

void
clear_screen(terminal_t *term, const char prompt[]);

void
clear_prompt(terminal_t *term);

void
delete_char(terminal_t *term);

void
end_of_line(terminal_t *term);

int
forward_char(terminal_t *term);

void
forward_word(terminal_t *term);

void
prompt_set_text(wchar_t newdata[], terminal_t *term);

#endif /* PROMPT_ACTION_H */
