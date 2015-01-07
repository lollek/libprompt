#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cutpaste.h"

typedef struct ll
{
  struct ll *next;
  char *text;
}
textlink_t;

static textlink_t *root = NULL;
static size_t current_killringsize = 0;

static int
kill_init_root(void)
{
  root = malloc(sizeof *root);
  if (root == NULL)
    return 1;
  memset(root, 0, sizeof *root);
  return 0;
}

static void
kill_unshift_oldest(void)
{
  textlink_t *deleteptr = root;
  textlink_t *endptr = NULL;

  while (deleteptr->next->next != NULL)
    deleteptr = deleteptr->next;

  endptr = deleteptr;
  deleteptr = deleteptr->next;

  endptr->next = NULL;
  free(deleteptr->text);
  free(deleteptr);

  current_killringsize--;
}

static void
kill_push_new(char text[])
{
  textlink_t *new = malloc(sizeof *new);
  if (new == NULL)
    return;

  new->next = root->next;
  new->text = strdup(text);
  if (new->text == NULL)
  {
    free(new);
    return;
  }

  root->next = new;
  current_killringsize++;
}

void
kill_line(char buf[], unsigned *counter, unsigned *pos)
{
  if (*counter == *pos)
    return;
  if (root == NULL && kill_init_root() != 0)
    return;

  if (current_killringsize == KILLSIZEMAX)
    kill_unshift_oldest();

  buf[*counter] = '\0';
  kill_push_new(buf + *pos);

  *counter = *pos;
  printf("\033[K\033[J");
}

void
kill_clear(void)
{
  while (root != NULL)
  {
    textlink_t *deleteptr = root;
    root = root->next;
    free(deleteptr->text);
    free(deleteptr);
  }
  current_killringsize = 0;
}
