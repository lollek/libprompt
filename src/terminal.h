#ifndef PROMPT_TERMINAL_H
#define PROMPT_TERMINAL_H

typedef unsigned length_t;

typedef struct
{
  char buf[BUFSIZE +1];
  length_t buflen;
  length_t cursorpos;
}
terminal_t;

#endif /* PROMPT_TERMINAL_H */
