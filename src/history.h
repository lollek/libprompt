#ifndef PROMPT_HISTORY_H
#define PROMPT_HISTORY_H

void
history_save(char *text);

void
history_prev_cmd(terminal_t *term);

void
history_next_cmd(terminal_t *term);

void
history_clear(void);

void
history_first_cmd(terminal_t *term);

void
history_last_cmd(terminal_t *term);

#endif /* PROMPT_HISTORY_H */
