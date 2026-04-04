# Notes — getchar()

## Source

K&R Chapter 1, sections 1.5.1 through 1.5.3.


## What it actually is

`getchar()` reads one character from the standard input stream (`stdin`)
and returns it as an `int`.

It does not know what a line is.
It does not know what a word is.
It only reads the next character available from the stream and advances
the stream position.

In most practical cases this corresponds to one byte of input,
but the abstraction in C is a character read from a stream.

## Why `int` and not `char`

EOF is not a character.
It is a special value returned by input functions to signal that no more
data is available.

Its value is typically `-1`, which falls outside the range of valid
character values.

`int` is wide enough to represent both:
- any valid character value
- the special EOF value

This is not a style preference.
It is a correctness requirement.


## The roles of `getchar()` inside a `while` condition

while ((c = getchar()) != EOF)

This single expression performs three operations:

1. Reads the next character from the stream
2. Stores the result in c
3. Tests whether the result is EOF

This pattern is fundamental in C because it combines input, assignment,
and termination control in one place.

## Open question

Terminal input is not delivered character by character in the way I first
imagined it.

When typing interactively, the program usually receives input only after
pressing Enter. That suggests buffering somewhere between the keyboard
and `getchar()`.

Questions to revisit later:

* Where exactly does this buffering happen?
* What is controlled by the OS or terminal driver?
* What is controlled by the C runtime (`stdio`)?
* Can this behavior be changed with raw or unbuffered input modes?


## Connection to later topics

`getchar()` is the simplest interface over a stream.

Higher-level input functions such as `scanf`, `fgets`, and file I/O
still rely on the same underlying idea:
data is read sequentially from a stream.

Understanding `getchar()` well should make later topics easier,
because the abstraction stays the same even when the interface changes.


## Observation

K&R introduces `getchar()` before explaining `stdin` in detail.

The idea seems to be:
use the abstraction first, then understand it more deeply through use.

That feels intentional.

It also reflects something broader in systems programming:
correct usage often comes before full understanding.

