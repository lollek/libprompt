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

/**
 * prompt - read a line from the user
 * @prompt	string to write as a prompt. NULL means no prompt.
 *
 * The line returned is allocated with malloc, so it must be freed when
 * no longer needed. The newline at the end of the string is removed.
 */
char
*prompt(const char *prompt);


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
