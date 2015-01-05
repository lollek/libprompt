#include <stdlib.h>
#include <stdio.h>

#include "prompt.h"

int main(void)
{
  for (;;)
  {
    char *line = prompt("> ");
    if (line == NULL)
      return 0;

    printf("You wrote '%s'\n", line);
    free(line);
  }

  return 0;
}
