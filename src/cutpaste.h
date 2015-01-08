#ifndef PROMPT_CUTPASTE_H
#define PROMPT_CUTPASTE_H

void
kill_line(char buf[], unsigned *counter, unsigned *pos);

void
kill_clear(void);

void
yank(char buf[], unsigned *counter, unsigned *pos);

void
backward_kill_line(char buf[], unsigned *counter, unsigned *pos);

#endif /* PROMPT_CUTPASTE_H */
