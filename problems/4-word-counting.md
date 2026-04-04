# Problem 4 — Word Counting

## Mistake 1 — Placing words++ outside the else block

**What I wrote**
if (state == OUT)
    words++;
state = IN;

**What I thought**
If state is OUT and we see any character, that is a new word.

**What actually happens**
Whitespace also evaluates the condition.
A space while `state == OUT` increments the counter.
Words that do not exist get counted.

**Fix**
The increment belongs inside the else block.
By the time it runs, we already know the character is not whitespace.

## Mistake 2 — Incomplete whitespace check

**What I wrote**
if (c == ' ')
    state = OUT;

**What I thought**
Spaces are the separator. That is enough.

**What actually happens**
Tab and newline are also whitespace but they fall through to the else block.
`"hello\tworld"` stays IN the entire time and counts as one word.
The program produces wrong results on any real text file
and compiles without warning.

**Fix**
if (c == ' ' || c == '\t' || c == '\n')
    state = OUT;

## Mistake 3 — Starting state as IN

**What I thought**
The initial state does not matter much.
The first character will set it correctly anyway.

**What actually happens**
If the input begins with a non-whitespace character,
the transition OUT→IN never fires for the first word.
It is already IN. The first word is never counted.

**Fix**
int state = OUT;
Before reading anything, we are not inside a word.
OUT is the only defensible starting point.

## Mistake 4 — Trying to count at the end of a word

**What I thought**
It feels more natural to count a word when it finishes —
when whitespace appears after non-whitespace.

**What actually happens**
EOF is not whitespace. If the input ends without whitespace,
the last word never triggers the end condition.
`"hello"` would return `0`.

**Fix**
Count at the beginning of a word, not the end.
The transition OUT→IN always has the information needed.
The transition IN→OUT does not — EOF may never provide it.

## Mistake 5 — Using char instead of int for c

**What I wrote**
char c;

**What I thought**
The program deals with characters. char seemed correct.

**What actually happens**
Same problem as in previous programs.
EOF cannot be reliably represented in char on all systems.
The loop may never terminate.

**Fix**
int c;

## Mistake 6 — Thinking "hello,world" should return 2

**What I thought**
A comma separates two words. The program should count 2.

**What actually happens**
The comma is not whitespace under the current definition.
The program never transitions to OUT between "hello" and "world".
It returns 1. This is correct behavior given the definition used.

The program is not wrong. The expectation was wrong.
Changing the result would require changing the definition of OUT,
which would change the contract of the program.

**No fix needed — wrong expectation, not wrong program.**

## Mistake 7 — Forgetting that state persists across iterations

**What I thought**
Each iteration of the loop is independent.
State resets somehow between characters.

**What actually happens**
State is declared outside the loop.
It holds its value across every iteration by design.
That persistence is the entire point — without it,
transitions are undetectable.

This is obvious in retrospect but took a moment to internalize
because the previous programs never needed memory between iterations.
