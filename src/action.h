#ifndef PROMPT_ACTION_H
#define PROMPT_ACTION_H

#include "terminal.h"

/* printc - add a characters to terminal
 * @ch		character to add
 * @term	current terminal
 *
 * Try to print the given characters to terminal
 * Returns the number of characters printed
 */
size_t
printc(char ch, terminal_t *term);

/* prints - add several characters to terminal
 * @str		string to add
 * @term	current terminal
 *
 * Tries to add the string to the terminal
 * Returns the number of characters printed
 */
size_t
prints(const char *str, terminal_t *term);

void
backward_delete_char(terminal_t *term);

void
backward_word(terminal_t *term);

void
beginning_of_line(terminal_t *term);

void
backward_char(terminal_t *term);

void
clear_screen(terminal_t *term, const char *prompt);

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
prompt_set_text(char newdata[], terminal_t *term);

#endif /* PROMPT_ACTION_H */
