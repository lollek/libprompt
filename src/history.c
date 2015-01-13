#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "action.h"
#include "terminal.h"

#include "history.h"

typedef struct dll
{
  struct dll *next;
  struct dll *prev;
  char *text;
}
histlink_t;

static histlink_t *root = NULL;
static histlink_t *current = NULL;
static size_t current_histsize = 0;

static char *tmpline = NULL;

static int
history_init_root(void)
{
  root = malloc(sizeof *root);
  if (root == NULL)
    return 1;
  memset(root, 0, sizeof *root);
  current = root;
  return 0;
}

static void
history_unshift_oldest(void)
{
  histlink_t *deleteptr = root->next;

  root->next = deleteptr->next;
  root->next->prev = root;

  free(deleteptr->text);
  free(deleteptr);

  current_histsize--;
}

static void
history_push_new(char *text)
{
  current->next = malloc(sizeof *current);
  if (current->next == NULL)
    return;

  current->next->next = NULL;
  current->next->prev = current;
  current->next->text = strdup(text);
  if (current->next->text == NULL)
  {
    free(current->next);
    current->next = NULL;
    return;
  }

 current = current->next;
 current_histsize++;
}

static int
history_save_tmpline(terminal_t *term)
{
  if (tmpline != NULL)
    free(tmpline);

  tmpline = malloc(term->buflen +1);
  if (tmpline == NULL)
    return 1;

  memcpy(tmpline, term->buf, term->buflen);
  tmpline[term->buflen] = '\0';
  return 0;
}

void
history_save(char *text)
{
  if (root == NULL && history_init_root() != 0)
    return;

  while (current->next != NULL)
    current = current->next;

  if (current != root && !strcmp(text, current->text))
    return;

  if (current_histsize == HISTSIZEMAX)
    history_unshift_oldest();
  history_push_new(text);
}

void
history_prev_cmd(terminal_t *term)
{
  if (root == NULL || current == root)
  {
    putchar('\a');
    return;
  }

  if (current->next == NULL && history_save_tmpline(term) != 0)
    return;

  prompt_set_text(current->text, term);
  current = current->prev;
}

void
history_next_cmd(terminal_t *term)
{
  if (root == NULL || current->next == NULL)
  {
    putchar('\a');
    return;
  }

  current = current->next;
  prompt_set_text(current->next ? current->next->text : tmpline, term);
}

void
history_clear(void)
{
  while (root != NULL)
  {
    histlink_t *deleteptr = root;
    root = root->next;
    if (deleteptr->text != NULL)
      free(deleteptr->text);
    free(deleteptr);
  }

  current = NULL;
  current_histsize = 0;

  if (tmpline != NULL)
  {
    free(tmpline);
    tmpline = NULL;
  }
}

void
history_first_cmd(terminal_t *term)
{
  if (root == NULL || current == root)
  {
    putchar('\a');
    return;
  }

  if (current->next == NULL && history_save_tmpline(term) != 0)
    return;

  current = root;
  prompt_set_text(current->next->text, term);
}

void
history_last_cmd(terminal_t *term)
{
  if (root == NULL || current->next == NULL)
  {
    putchar('\a');
    return;
  }

  while (current->next != NULL)
    current = current->next;
  prompt_set_text(tmpline, term);
}
