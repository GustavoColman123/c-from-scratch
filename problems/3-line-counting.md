# Problem 3 — Line Counting

## Mistake 1 — Using char instead of int

**What I wrote**
char c;

**What I thought**
The program only deals with characters. `char` seemed obvious.

**What actually happens**
`char` cannot reliably represent EOF.
On systems where `char` is unsigned, `−1` wraps to `255`,
which is a valid character value. The loop may never terminate.

**Fix**
int c;

## Mistake 2 — Missing parentheses in while condition

**What I wrote**
while (c = getchar() != EOF)

**What I thought**
Assignment happens first, then comparison.

**What actually happens**
`!=` binds tighter than `=`. The expression becomes:
c = (getchar() != EOF)

`c` stores `0` or `1`. The character is gone.
`c == '\n'` can never be true. The program counts nothing.
The compiler does not warn.

**Fix**
while ((c = getchar()) != EOF)

## Mistake 3 — Trying to skip storing the character

**What I thought**
Day 02 worked without storing the result. This should too.

**What actually happens**
Day 02 did not care about the value.
This program does. Without storing it, there is nothing to compare against `'\n'`.

**Fix**
Store the result. The value is the point.


## Mistake 4 — Thinking '\n' was special syntax

**What I thought**
`'\n'` was handled differently by the compiler, like an escape sequence
the language intercepts before the comparison runs.

**What actually happens**
`'\n'` is just the integer `10`.
The `if` compares two integers. Nothing special happens.
The notation exists for readability, not for behavior.

**No fix needed just a wrong mental model that needed replacing.**


## Subtle issue — Last line without newline

**Observation**
Input `hello\nworld` returns `1`, not `2`.

**Reason**
The program counts `'\n'` delimiters, not logical lines.
A trailing segment without a newline is never counted.

**Implication**
The result can differ from tools like `wc -l` depending on the file.
A correct line counter would need extra logic to detect unterminated lines.
This program does not have that logic and does not claim to.
