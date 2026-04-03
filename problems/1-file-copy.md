Que opinas? Aquí va el de problems 


# Problem 1 — File Copy with getchar()

## The program (final version)

```c
#include <stdio.h>

int main(void) {
    int c;
    while ((c = getchar()) != EOF)
        putchar(c);
    return 0;
}
```

## Mistakes I made while getting here

### Mistake 1 — Used char instead of int

**What I wrote:**
```c
char c;
while ((c = getchar()) != EOF)
    putchar(c);
```

**What I thought:**
getchar reads a character so I store it in a char. Makes sense.

**What actually happens:**
On systems where char is unsigned: the byte 0xFF (255) gets stored as 255.
EOF is -1. They never match. The loop never terminates on binary input.

On systems where char is signed: the byte 0xFF becomes -1 after conversion.
EOF is also -1. The loop exits early on valid data. Silent data corruption.

**The fix:**
`int c` — large enough to hold 0–255 AND -1 without collision.

**How long it took to understand:** longer than I expected.
I kept thinking "it probably works in practice." It does — until it doesn't,
and then you have a bug in a binary file parser with no obvious cause.

### Mistake 2 — Missing parentheses in while condition

**What I wrote:**
```c
while (c = getchar() != EOF)
```

**What I thought:**
The assignment happens, then the comparison. Looked fine at a glance.

**What actually happens:**
Operator precedence: `!=` binds tighter than `=`.

So it evaluates as:
```c
while (c = (getchar() != EOF))
```

`getchar() != EOF` evaluates to 0 or 1 (boolean).
THAT value gets assigned to `c`.

So `c` is always 0 or 1, never the actual byte.
`putchar(c)` outputs control characters the whole time.

The loop still terminates correctly on EOF — which makes this extra
confusing to debug. The behavior looks almost right.

**The fix:**
```c
while ((c = getchar()) != EOF)
```

Inner parentheses force assignment before comparison.

**Lesson:** in C, operator precedence does not match reading order.
Always trace it manually when assignment is inside a condition.

### Mistake 3 — getchar() inside the loop body

**What I wrote:**
```c
int c = 0;
while (c != EOF) {
    c = getchar();
    putchar(c);
}
```

**What I thought:**
Reads first, then checks. Same thing, right?

**What actually happens:**
When getchar() returns EOF, putchar(c) still runs before the loop
can re-evaluate the condition.

`putchar(-1)` — the behavior here is implementation-defined.
On most systems it outputs something garbage or wraps around.
Either way it is wrong.

Also: `c` is initialized to 0, not EOF, so the first condition check
passes regardless. That part is fine — but it only works by accident.
If I had initialized `c` to -1 (EOF), the loop would never enter.

**The fix:**
Move the read into the condition itself:
```c
while ((c = getchar()) != EOF)
```
Now getchar(), assignment, and comparison happen atomically in the
condition. putchar() only runs when c is a valid byte.

### Mistake 4 — Thought Ctrl+D "sends" EOF

**What I thought:**
Ctrl+D is the EOF key. Pressing it sends the EOF character to the program.

**What actually happens:**
There is no EOF character. Ctrl+D tells the terminal driver to flush
its buffer immediately.

If there are characters in the buffer: they get sent to the program.
No EOF yet. The program reads them normally.

If the buffer is empty (or after a flush): the OS closes the read end.
`getchar()` then returns -1. That is what we call "receiving EOF."

So pressing Ctrl+D twice on a non-empty line: first press flushes, second
press (on now-empty buffer) produces EOF.

This confused me when I was testing interactively and EOF seemed to arrive
at the wrong time.

### Mistake 5 — Assumed output appears character by character

**What I thought:**
`putchar(c)` writes immediately to the screen. Each character appears
as it is processed.

**What actually happens:**
`putchar()` writes to stdout, which is buffered.
In interactive terminals, stdout is often line-buffered — output is held
until a newline is written or the buffer fills.

So if I type "hello" and press Enter, the output appears after the newline
is processed — not as I type each letter.

If stdout is redirected to a file or pipe, it may be fully buffered,
meaning nothing appears until the buffer fills or the program exits.

This caused me to think the program was broken during testing when it was
actually just waiting to flush.

**The fix for testing:** add `fflush(stdout)` if you need to see output
immediately. Or just understand that buffering is normal.
## Final check before committing

- [x] `int c`, not `char c`
- [x] assignment inside condition with correct parentheses
- [x] putchar only runs when c is a valid byte
- [x] return 0 present
- [x] tested mentally with echo, file redirect, yes pipe
