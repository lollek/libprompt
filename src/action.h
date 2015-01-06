#ifndef PROMPT_ACTION_H
#define PROMPT_ACTION_H

void
handle_printables(int ch, char buf[], unsigned *counter, unsigned *pos);

void
backward_delete_char(char buf[], unsigned *counter, unsigned *pos);

void
backward_word(char buf[], unsigned *pos);

void
beginning_of_line(unsigned *pos);

void
backward_char(unsigned *pos);

void
clear_screen(char buf[], unsigned *counter, unsigned *pos, const char *prompt);

void
delete_char(char buf[], unsigned *counter, unsigned *pos);

void
end_of_line(char buf[], unsigned *counter, unsigned *pos);

int
forward_char(char buf[], unsigned *counter, unsigned *pos);

void
forward_word(char buf[], unsigned *counter, unsigned *pos);

void
prompt_set_text(char newdata[], char buf[], unsigned *counter, unsigned *pos);

#endif /* PROMPT_ACTION_H */
