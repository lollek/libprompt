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
