Y este va en day01 
# Day 01 — getchar(), EOF, and the copy program

## What I worked on

K&R Exercise 1-1 area. Reading about getchar() and writing the simplest
possible program that copies stdin to stdout.

## What clicked immediately

The stream abstraction made sense right away. The idea that `./program`,
`./program < file.txt` and `echo hello | ./program` are all the same code
just with stdin pointing to different things — that was clean and obvious
once I saw it.

## What took longer than expected

### The int vs char thing

My first instinct was `char c`. It just felt right. You are reading a
character, so you store it in a char. Seemed obvious.

Then I read that `getchar()` returns `int` and I thought okay, implicit
conversion will handle it. It probably works most of the time. I almost
moved on.

What actually made it click was thinking about `0xFF`. If char is signed,
`0xFF` stored in a `char` becomes -1. EOF is also -1. The program would
stop on a perfectly valid byte and I would have no idea why.

That is not a theoretical edge case. Binary files contain `0xFF` constantly.

After that I stopped thinking about it as "oh the type is slightly wrong"
and started thinking about it as "this is a real bug waiting to happen on
real data."

### The parentheses in the while condition

I knew `=` had lower precedence than `!=` but I did not immediately see
why that broke everything.

I traced it manually:

```c
while (c = getchar() != EOF)
```

Step by step:
- `getchar()` returns, say, 104 (the letter 'h')
- `104 != EOF` → `104 != -1` → true → evaluates to 1
- `c = 1`
- loop body runs with `c == 1`, not `c == 104`
- `putchar(1)` → prints a control character, not 'h'

So the program would run but output garbage. And it would be hard to debug
because the loop would still terminate correctly on EOF. Just the output
would be wrong the whole time.

The fix is obvious once you see it — just add the inner parentheses. But
understanding WHY it breaks required tracing the precedence manually.

### EOF on the terminal

I kept asking myself: where does EOF come from on a keyboard? There is no
EOF key.

Ctrl+D on Linux does not "send EOF". What it does is flush the terminal
buffer immediately. If the buffer is empty when you press Ctrl+D, the OS
closes the read end and getchar() gets -1.

So Ctrl+D on a non-empty line just flushes. Ctrl+D on an empty line (or
after flushing) produces EOF. That was slightly confusing until I tested it.

## Decisions I made consciously

**No curly braces on the while loop.**
One statement in the body. Braces add noise without adding clarity here.
When I extend this with error handling later, I will add them.

**No putchar(getchar()) shortcut.**
Technically works. But I lose `c` as a variable I can inspect, check, or
use later. Keeping `c` explicit gives me control.

**return 0 at the end.**
Not strictly required in C99+, but I want the habit. Makes intent explicit.

## Things I want to understand better

- What exactly happens at the OS level when stdin is a pipe vs a file
- How buffering behavior differs between environments (terminal vs file vs pipe),
  and how much of it is defined by the C standard vs implementation-specific
- How ferror and feof interact Under what conditions feof and ferror are set, and whether they
  can both be true simultaneously
- What happens to the buffer if the process is killed mid-read

## Time spent

Reading + thinking: ~2 h
Writing the program: 10 min
Writing this log: 30 min

The code was the fast part. Understanding why it is written that way was
the actual work.

## What this day changed

Before this, I thought of input as "characters coming from somewhere".

Now I see it as:
- a stream abstraction
- a sequence of bytes
- a system where control signals (like EOF) are separate from data

Most of the difficulty was not writing the program, but correcting
my mental model of how input/output actually works.
