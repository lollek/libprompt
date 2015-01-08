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
    char *line = prompt("> ");
    if (line == NULL)
      break;

    printf("You wrote '%s'\n", line);
    free(line);
  }

  prompt_free();
  return 0;
}
