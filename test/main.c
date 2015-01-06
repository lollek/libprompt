#include <stdlib.h>
#include <stdio.h>

#include "../src/prompt.h"

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
