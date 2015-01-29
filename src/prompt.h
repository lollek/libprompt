/**
 * Copyright (c) 2015, Olle Kvarnström olle+libprompt@iix.se
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef PROMPT_PROMPT_H
#define PROMPT_PROMPT_H

#include <wchar.h>

#define PROMPT_BUF_SIZE BUFSIZE +1

/**
 * prompt - read a line from the user
 * @prompt	string to write as a prompt. NULL means no prompt.
 *
 * The line returned is allocated with malloc, so it must be freed when
 * no longer needed. The newline at the end of the string is removed.
 */
char *
prompt(const char *prompt);

/**
 * prompt_r - read a line from the user
 * @prompt	string to write as a prompt. NULL means no prompt
 * @buffer	buffer to store the return string in. Needs to be able to store
 * 		PROMPT_BUF_SIZE characters
 *
 * prompt_r works like prompt except that instead of returning a char array
 * stored on the heap, it stores the data in @buffer and returns a reference to
 * it
 */
wchar_t *
prompt_r(const char *prompt, wchar_t *buffer);


/**
 * prompt_free - free all data used by prompt
 *
 * This will free all memory on heap used by the prompt, not including anything
 * returned to the user (i.e. the line from prompt).
 * This functions is perfectly safe to call and then continue using prompt.
 */
void
prompt_free(void);

#endif /* PROMPT_PROMPT_H */
