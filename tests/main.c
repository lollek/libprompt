#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#  include "../src/prompt.h"
#else
#  include <prompt.h>
#endif

int main(void)
{
  for (;;)
  {
    char buf[PROMPT_BUF_SIZE];
    char *line = prompt_r("> ", buf);
    if (line == NULL)
      break;

    printf("You wrote '%s'\n", line);
  }

  prompt_free();
  return 0;
}
