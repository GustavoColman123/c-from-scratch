# Day 02 — Character Counting

## What I worked on

Implemented a program that counts characters from stdin until EOF
using a while loop and a long accumulator.

## What changed from Day 01

The previous program read each character to print it back.
This one reads each character but does not retain its value.

The loop condition is the same. What changed is the body:
instead of putchar(c), there is only ++nc.
Because the value is never used, there is no reason to store it.

## Key insight

getchar() serves two purposes at once here.
It advances the stream and returns a value we can test.

The return value is not stored, only evaluated in the loop condition.

The value of the character is irrelevant to the logic.
Only the continuation of the stream matters.

## Why long

A text file can have billions of characters.
int overflows around 2 billion on most systems.

long extends the range of the counter, reducing the risk of overflow
when processing large inputs.

## The thing that actually clicked

I kept thinking of input as a collection of characters.
This program treats it as a stream that either continues or ends.

The program no longer asks "what is this character",
but rather "does input continue".

## Time

~30 min
