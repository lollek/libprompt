#ifndef PROMPT_ACTION_H
#define PROMPT_ACTION_H

void
handle_printables(int ch, char *buf, unsigned *counter, unsigned *pos);

void
handle_backspace(unsigned *counter, unsigned *pos);

void
beginning_of_line(unsigned *pos);

void
backward_char(unsigned *pos);

void
end_of_line(unsigned *counter, unsigned *pos);

void
forward_char(unsigned *counter, unsigned *pos);

#endif /* PROMPT_ACTION_H */
