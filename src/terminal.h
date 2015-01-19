#ifndef PROMPT_TERMINAL_H
#define PROMPT_TERMINAL_H

#include <wchar.h>

typedef struct
{
  wchar_t buf[BUFSIZE +1];
  size_t buflen;
  size_t cursorpos;
}
terminal_t;

#endif /* PROMPT_TERMINAL_H */
