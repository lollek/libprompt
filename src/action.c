#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "action.h"

size_t
prints(const char *str, terminal_t *term)
{
  size_t chars_to_write;
  size_t buf_space_left;

  if (term->buflen == BUFSIZE)
  {
    putchar('\a');
    return 0;
  }

  chars_to_write = strlen(str);
  buf_space_left = BUFSIZE - term->buflen;
  if (chars_to_write > buf_space_left)
    chars_to_write = buf_space_left;

  if (term->cursorpos == term->buflen)
  {
    fwrite(str, sizeof(char), chars_to_write, stdout);
    memcpy(term->buf + term->buflen, str, chars_to_write);
    term->buflen += chars_to_write;
    term->cursorpos = term->buflen;
    return chars_to_write;
  }

  else
  {
    char movebackbuf[BUFSIZE];
    size_t i;

    memmove(term->buf + term->cursorpos + chars_to_write,
            term->buf + term->cursorpos,
            term->buflen - term->cursorpos);
    memcpy(term->buf + term->cursorpos, str, chars_to_write);
    term->buflen += chars_to_write;

    fwrite(term->buf + term->cursorpos, sizeof(char),
           term->buflen - term->cursorpos, stdout);
    term->cursorpos += chars_to_write;

    for (i = term->cursorpos; i < term->buflen; ++i)
      movebackbuf[i - term->cursorpos] = '\b';
    fwrite(movebackbuf, sizeof(char), term->buflen - term->cursorpos, stdout);
    return chars_to_write;
  }
}

size_t
printc(char ch, terminal_t *term)
{
  char buf[2];
  buf[0] = ch;
  buf[1] = '\0';
  return prints(buf, term);
}

void
backward_delete_char(terminal_t *term)
{
  if (term->cursorpos == 0)
    putchar('\a');
  else if (term->cursorpos == term->buflen)
  {
    fwrite("\b \b", sizeof(char), 3, stdout);
    term->cursorpos -= 1;
    term->buflen -= 1;
  }
  else
  {
    char outbuf[BUFSIZE * 3];
    char *outbufpos = outbuf;
    length_t i;

    memmove(term->buf + term->cursorpos -1, term->buf + term->cursorpos,
            term->buflen - term->cursorpos);
    term->buflen -= 1;
    term->cursorpos -= 1;

    *outbufpos = '\b';
    outbufpos++;

    memcpy(outbufpos, term->buf + term->cursorpos,
           term->buflen - term->cursorpos);
    outbufpos += term->buflen - term->cursorpos;

    memcpy(outbufpos, " \b", 2);
    outbufpos += 2;

    for (i = term->cursorpos; i < term->buflen; ++i)
    {
      *outbufpos = '\b';
      outbufpos++;
    }

    fwrite(outbuf, sizeof(char), outbufpos - outbuf, stdout);
  }
}

void
backward_word(terminal_t *term)
{
  if (term->cursorpos == 0)
    return;
  do
    backward_char(term);
  while (term->cursorpos > 0 &&
         (!isalnum(term->buf[term->cursorpos])
          || isalnum(term->buf[term->cursorpos -1])));
}

void
beginning_of_line(terminal_t *term)
{
  while (term->cursorpos > 0)
    backward_char(term);
}

void
backward_char(terminal_t *term)
{
  if (term->cursorpos == 0)
    putchar('\a');
  else
  {
    putchar('\b');
    term->cursorpos -= 1;
  }
}

void
clear_screen(terminal_t *term, const char *prompt)
{
  fwrite("\033[2J\033[:H", sizeof(char), sizeof("\033[2J\033[;H"), stdout);

  if (prompt != NULL)
    printf("%s", prompt);

  if (term->buflen != 0)
  {
    char tmpbuf[BUFSIZE *2];
    unsigned i = 0;
    for (i = 0; i < term->buflen; ++i)
      tmpbuf[i] = term->buf[i];
    for (i += term->cursorpos; i - term->buflen < term->buflen; ++i)
      tmpbuf[i - term->buflen] = term->buf[i - term->buflen];
    fwrite(tmpbuf, sizeof(char), i, stdout);
  }
}

void
clear_prompt(terminal_t *term)
{
  beginning_of_line(term);
  fwrite("\033[K\033[J", sizeof(char), sizeof("\033[K\033[J"), stdout);
}

void
delete_char(terminal_t *term)
{
  if (forward_char(term) == 0)
    backward_delete_char(term);
}

void
end_of_line(terminal_t *term)
{
  while (term->cursorpos < term->buflen)
    forward_char(term);
}

int
forward_char(terminal_t *term)
{
  if (term->cursorpos < term->buflen)
  {
    putchar(term->buf[term->cursorpos++]);
    return 0;
  }
  else
  {
    putchar('\a');
    return 1;
  }
}

void
forward_word(terminal_t *term)
{
  while (term->cursorpos < term->buflen)
  {
    forward_char(term);
    if (!isalnum(term->buf[term->cursorpos])
        && isalnum(term->buf[term->cursorpos -1]))
      return;
  }
}

void
prompt_set_text(char newdata[], terminal_t *term)
{
  size_t newdatasiz = strlen(newdata);

  clear_prompt(term);

  strcpy(term->buf, newdata);
  fwrite(term->buf, sizeof(char), newdatasiz, stdout);
  term->cursorpos = term->buflen = newdatasiz;
}
