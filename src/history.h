#ifndef PROMPT_HISTORY_H
#define PROMPT_HISTORY_H

void
history_save(char *text);

void
history_prev_cmd(char buf[], unsigned *counter, unsigned *pos);

void
history_next_cmd(char buf[], unsigned *counter, unsigned *pos);

void
history_delete(void);

#endif /* PROMPT_HISTORY_H */
