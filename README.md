libprompt
=========
libprompt aims to be a lite version of libreadline, or more specifically, readline's `char *readline(const char *)` function.

## Implemented Features
### Moving
* beginning-of-line (C-a) - Move to the start of the prompt.
* end-of-line (C-e) - Move to the end of the prompt.
* forward-char (C-f, arrow right) - Move forward a character.
* backward-char (C-b, arrow left) - Move back a character.
* forward-word (M-f) - Move forward to the end of the this/the next word.
* backward-word (M-b) - Move back to the start of the this/the previous word.
* clear-screen (C-l) - Clear the screen.

### Editing
* delete-char (C-d) - Delete character at position, or signal end of line if there are no characters.
* backward-delete-char (backspace) - Delete character to the left of the current position.
* kill-line (C-k) - Kill from cursor to the end of the prompt and put it in the kill-ring.
* yank (C-y) - Yank the top of the kill-ring and put it at the cursor.
* backward-kill-line (C-u) - Kill from cursor to the start of the prompt and put it in the kill-ring.

### History
* previous-command (C-p, arrow up) - Show previous entered command.
* next-command (C-n, arrow down) - Show next entered command.
* beginning-of-history (M-<) - Show the oldest saved command.
* end-of-history (M->) Show the newest saved command.

## To be implemented
### Editing
* kill-word (M-d) - Kill from cursor to the end of the current word.
* yank-pop (M-y) - Rotate the kill-ring to show some less recent data. Only works after yank or yank-pop.
* repeat-char (M-0..9) - Type a number and then press a character, to show that many characters. E.g M-1,0,c will output 10 c's
