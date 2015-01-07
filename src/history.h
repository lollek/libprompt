#ifndef PROMPT_HISTORY_H
#define PROMPT_HISTORY_H

void
history_save(char *text);

void
history_prev_cmd(char buf[], unsigned *counter, unsigned *pos);

void
history_next_cmd(char buf[], unsigned *counter, unsigned *pos);

void
history_clear(void);

void
history_first_cmd(char buf[], unsigned *counter, unsigned *pos);

void
history_last_cmd(char buf[], unsigned *counter, unsigned *pos);

#endif /* PROMPT_HISTORY_H */
