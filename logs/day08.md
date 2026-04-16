# Day 08 — Call by Value and Character Arrays

## What I built

Implemented the longest line program from K&R Chapter 1.
This day covers two sections together: 1.8 (call by value) and 1.9 (character arrays).
Both concepts are required to understand the program, they could not be separated cleanly.

## What changed from Day 07

Day 07 introduced functions with scalar arguments.
The function received copies. Nothing in the caller could be modified.

This program passes arrays. The behavior looks the same on the surface
but the mechanics are different, and the difference matters.

## What I had to define before writing code

- What "call by value" actually means for scalars vs arrays
- How strings are represented in C
- How three functions divide responsibility across the program

## What clicked

Scalars and arrays are not the same when passed to functions.

A scalar is copied. The function gets its own version.
An array is not copied — what gets passed is the address of the first element.
The function can reach into the caller's memory and modify it directly.

This is still call by value. The value being copied is the address, not the array.
That distinction is subtle and easy to miss.

## Key difference in thinking

Day 07: the function computes and returns. The caller's data is untouched.
Day 08: the function writes into memory the caller owns.
The return value coordinates. The array carries the result.

## Mistakes / confusions

Assumed that passing an array meant passing a copy of its contents.
The program appeared to work but the mental model was wrong.
Had to reason through what actually gets copied before the behavior made sense.

The null terminator `'\0'` is not optional.
Without it, the string has no defined end and any function that reads it
will continue past the intended boundary.

## Time

60 min
