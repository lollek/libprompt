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

#endif /* PROMPT_PROMPT_H */
