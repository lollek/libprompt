#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

#include "action.h"

#include "cutpaste.h"

typedef struct ll
{
  struct ll *next;
  wchar_t *text;
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
kill_push_new(wchar_t text[], size_t length)
{
  textlink_t *new = malloc(sizeof *new);
  if (new == NULL)
    return;

  new->next = root->next;
  new->text = malloc(sizeof(wchar_t) * (length +1));
  if (new->text == NULL)
  {
    free(new);
    return;
  }
  wmemcpy(new->text, text, length +1);

  root->next = new;
  current_killringsize++;
}

void
kill_line(terminal_t *term)
{
  if (term->cursorpos == term->buflen)
    return;
  if (root == NULL && kill_init_root() != 0)
    return;

  if (current_killringsize == KILLSIZEMAX)
    kill_unshift_oldest();

  kill_push_new(term->buf + term->cursorpos, term->buflen - term->cursorpos);
  term->buflen = term->cursorpos;
  fwrite("\033[K\033[J", sizeof(char), sizeof("\033[K\033[J"), stdout);
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
yank(terminal_t *term)
{
  wchar_t *pastedata;
  size_t pastedatalen;

  if (root == NULL)
    return;

  pastedata = root->next->text;
  pastedatalen = wcslen(pastedata);
  if (pastedatalen + term->buflen > BUFSIZE)
  {
    putchar('\a');
    pastedatalen = BUFSIZE - term->buflen;
  }

  if (term->cursorpos != term->buflen)
  {
    size_t i;

    wmemmove(term->buf + term->cursorpos + pastedatalen,
             term->buf + term->cursorpos, term->buflen - term->cursorpos);
    wmemcpy(term->buf + term->cursorpos, pastedata, pastedatalen);
    term->buflen += pastedatalen;

    fwrite(term->buf + term->cursorpos, sizeof(wchar_t),
           term->buflen - term->cursorpos, stdout);
    term->cursorpos += pastedatalen;

    for (i = term->cursorpos; i < term->buflen; ++i)
      putchar('\b');
  }
  else
  {
    wmemcpy(term->buf + term->buflen, pastedata, pastedatalen);
    fwrite(term->buf + term->cursorpos, sizeof(wchar_t), pastedatalen, stdout);
    term->cursorpos = term->buflen = term->buflen + pastedatalen;
  }
}

void
backward_kill_line(terminal_t *term)
{
  wchar_t tempbuf[BUFSIZE];
  size_t tempbufsiz;
  if (term->cursorpos == 0)
  {
    putchar('\a');
    return;
  }

  if (root == NULL && kill_init_root() != 0)
    return;

  wmemcpy(tempbuf, term->buf, term->cursorpos);
  tempbufsiz = term->cursorpos;
  wmemmove(term->buf, term->buf + term->cursorpos,
           term->buflen - term->cursorpos);

  term->buflen -= term->cursorpos;
  clear_prompt(term);

  fwrite(term->buf, sizeof(wchar_t), term->buflen, stdout);
  term->cursorpos = term->buflen;
  beginning_of_line(term);

  if (current_killringsize == KILLSIZEMAX)
    kill_unshift_oldest();
  kill_push_new(tempbuf, tempbufsiz);
}

void
kill_word(terminal_t *term)
{
  wchar_t tempbuf[BUFSIZE];
  size_t tempbufsiz;
  size_t cut_from = term->cursorpos;

  if (term->cursorpos == term->buflen)
    return;
  if (root == NULL && kill_init_root() != 0)
    return;

  forward_word(term);
  tempbufsiz = term->cursorpos - cut_from;
  wmemcpy(tempbuf, term->buf + cut_from, tempbufsiz);
  wmemmove(term->buf + cut_from, term->buf + term->cursorpos,
           term->buflen - term->cursorpos);
  term->buflen -= term->cursorpos - cut_from;

  while (term->cursorpos > cut_from)
    backward_char(term);
  fwrite(term->buf, sizeof(wchar_t), term->buflen, stdout);
  fwrite("\033[J\033[K", sizeof(char), sizeof("\033[J\033[K"), stdout);

  term->cursorpos = term->buflen;
  while (term->cursorpos > cut_from)
    backward_char(term);

  if (current_killringsize == KILLSIZEMAX)
    kill_unshift_oldest();
  kill_push_new(tempbuf, tempbufsiz);
}
