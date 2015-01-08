#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "action.h"

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

void
yank(char buf[], unsigned *counter, unsigned *pos)
{
  char *pastedata;
  size_t pastedatalen;

  if (root == NULL)
    return;

  pastedata = root->next->text;
  pastedatalen = strlen(pastedata);
  if (pastedatalen + *counter > BUFSIZE)
  {
    putchar('\a');
    pastedatalen = BUFSIZE - *counter;
  }

  if (*pos != *counter)
  {
    unsigned i;

    memmove(buf + *pos + pastedatalen, buf + *pos, *counter - *pos);
    strncpy(buf + *pos, pastedata, pastedatalen);
    *counter += pastedatalen;
    buf[*counter] = '\0';

    printf("%s", buf + *pos);
    *pos += pastedatalen;

    for (i = *pos; i < *counter; ++i)
      putchar('\b');
  }
  else
  {
    strncpy(buf + *counter, pastedata, pastedatalen);
    buf[*counter + pastedatalen] = '\0';
    printf("%s", buf + *counter);
    *pos = *counter = *counter + pastedatalen;
  }
}

void
backward_kill_line(char buf[], unsigned *counter, unsigned *pos)
{
  char tempbuf[BUFSIZE +1];
  if (*pos == 0)
  {
    putchar('\a');
    return;
  }

  if (root == NULL && kill_init_root() != 0)
    return;

  memcpy(tempbuf, buf, *pos);
  tempbuf[*pos] = '\0';

  memmove(buf, buf + *pos, *pos);
  *counter -= *pos;
  clear_prompt(pos);

  buf[*counter] = '\0';
  printf("%s", buf);
  *pos = *counter;
  beginning_of_line(pos);

  if (current_killringsize == KILLSIZEMAX)
    kill_unshift_oldest();
  kill_push_new(tempbuf);
}

void
kill_word(char buf[], unsigned *counter, unsigned *pos)
{
  char tempbuf[BUFSIZE +1];
  unsigned cut_from = *pos;

  if (*pos == *counter)
    return;
  if (root == NULL && kill_init_root() != 0)
    return;

  forward_word(buf, counter, pos);
  memcpy(tempbuf, buf + cut_from, *pos - cut_from);
  tempbuf[*pos - cut_from] = '\0';

  memmove(buf + cut_from, buf + *pos, *counter - *pos);
  *counter -= *pos - cut_from;
  buf[*counter] = '\0';

  while (*pos > cut_from)
    backward_char(pos);
  printf("%s\033[J\033[K", buf);
  for (*pos = *counter; *pos > cut_from; backward_char(pos))
    ;

  if (current_killringsize == KILLSIZEMAX)
    kill_unshift_oldest();
  kill_push_new(tempbuf);
}
