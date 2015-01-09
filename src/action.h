#ifndef PROMPT_ACTION_H
#define PROMPT_ACTION_H

#include "terminal.h"

void
handle_printables(int ch, terminal_t *term);

void
backward_delete_char(terminal_t *term);

void
backward_word(char buf[], unsigned *pos);

void
beginning_of_line(unsigned *pos);

void
backward_char(unsigned *pos);

void
clear_screen(char buf[], unsigned *counter, unsigned *pos, const char *prompt);

void
clear_prompt(unsigned *pos);

void
delete_char(terminal_t *term);

void
end_of_line(terminal_t *term);

int
forward_char(terminal_t *term);

void
forward_word(terminal_t *term);

void
prompt_set_text(char newdata[], char buf[], unsigned *counter, unsigned *pos);

#endif /* PROMPT_ACTION_H */
