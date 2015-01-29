#include <stdio.h>
#include <ctype.h>
#include <wctype.h>

#include "action.h"

void
printc(wchar_t ch, terminal_t *term)
{
  if (term->buflen == BUFSIZE)
    putwchar(L'\a');
  else if (term->cursorpos != term->buflen)
  {
    wchar_t movebackbuf[BUFSIZE];
    const wchar_t *cpyfrom = term->buf + term->cursorpos;
    wchar_t *cpyto = term->buf + term->cursorpos +1;
    const size_t cpysiz = term->buflen - term->cursorpos;
    size_t i;

    wmemmove(cpyto, cpyfrom, cpysiz);
    term->buflen += 1;

    term->buf[term->cursorpos] = ch;
    fwrite(cpyfrom, sizeof(wchar_t), cpysiz +1, stdout);
    term->cursorpos += 1;

    for (i = term->cursorpos; i < term->buflen ; ++i)
      movebackbuf[i - term->cursorpos] = L'\b';
    fwrite(movebackbuf, sizeof(wchar_t), cpysiz, stdout);
  }
  else
  {
    putwchar(ch);
    term->buf[term->cursorpos] = ch;
    term->cursorpos += 1;
    term->buflen += 1;
  }
}

void
backward_delete_char(terminal_t *term)
{
  if (term->cursorpos == 0)
    putwchar(L'\a');
  else if (term->cursorpos == term->buflen)
  {
    fwrite(L"\b \b", sizeof(wchar_t), sizeof(L"\b \b"), stdout);
    term->cursorpos -= 1;
    term->buflen -= 1;
  }
  else
  {
    wchar_t outbuf[BUFSIZE * 3];
    wchar_t *outbufpos = outbuf;
    size_t i;

    wmemmove(term->buf + term->cursorpos -1, term->buf + term->cursorpos,
             term->buflen - term->cursorpos);
    term->buflen -= 1;
    term->cursorpos -= 1;

    *outbufpos = L'\b';
    outbufpos++;

    wmemcpy(outbufpos, term->buf + term->cursorpos,
            term->buflen - term->cursorpos);
    outbufpos += term->buflen - term->cursorpos;

    wmemcpy(outbufpos, L" \b", 2);
    outbufpos += 2;

    for (i = term->cursorpos; i < term->buflen; ++i)
    {
      *outbufpos = L'\b';
      outbufpos++;
    }

    fwrite(outbuf, sizeof(wchar_t), outbufpos - outbuf, stdout);
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
         (!iswalnum(term->buf[term->cursorpos])
          || iswalnum(term->buf[term->cursorpos -1])));
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
    putwchar(L'\a');
  else
  {
    putwchar(L'\b');
    term->cursorpos -= 1;
  }
}

void
clear_screen(terminal_t *term, const char *prompt)
{
  fwrite(L"\033[2J\033[:H", sizeof(wchar_t), sizeof(L"\033[2J\033[;H"), stdout);

  if (prompt != NULL)
    printf("%s", prompt);

  if (term->buflen != 0)
  {
    wchar_t tmpbuf[BUFSIZE *2];
    size_t i = 0;
    for (i = 0; i < term->buflen; ++i)
      tmpbuf[i] = term->buf[i];
    for (i += term->cursorpos; i - term->buflen < term->buflen; ++i)
      tmpbuf[i - term->buflen] = term->buf[i - term->buflen];
    fwrite(tmpbuf, sizeof(wchar_t), i, stdout);
  }
}

void
clear_prompt(terminal_t *term)
{
  beginning_of_line(term);
  fwrite(L"\033[K\033[J", sizeof(wchar_t), sizeof(L"\033[K\033[J"), stdout);
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
    putwchar(term->buf[term->cursorpos++]);
    return 0;
  }
  else
  {
    putwchar(L'\a');
    return 1;
  }
}

void
forward_word(terminal_t *term)
{
  while (term->cursorpos < term->buflen)
  {
    forward_char(term);
    if (!iswalnum(term->buf[term->cursorpos])
        && iswalnum(term->buf[term->cursorpos -1]))
      return;
  }
}

void
prompt_set_text(wchar_t newdata[], terminal_t *term)
{
  size_t newdatasiz = wcslen(newdata);

  clear_prompt(term);

  wcscpy(term->buf, newdata);
  fwrite(term->buf, sizeof(wchar_t), newdatasiz, stdout);
  term->cursorpos = term->buflen = newdatasiz;
}
