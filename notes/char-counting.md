# Notes — Character Counting

## Source

K&R Chapter 1, section 1.5.2.

## What it actually is

A program that increments a counter for every character read from stdin.

It does not use the value beyond testing for EOF.
Every character that passes through the loop is counted unconditionally.

## Why the character is not stored

The value is not needed after the condition.

`getchar()` is used to:
- advance the stream
- determine whether input continues

The result is not stored because it is not part of the logic.

This is one of the first examples where a function’s return value
is used only for control flow, not for data processing.

## Why long and not int

The choice of type is driven by the expected range of the result,
not by the type of the input.

The program processes characters, but stores a count.
The count can grow independently of the size of a single character.

`int` may overflow for large inputs.
`long` extends the range without changing the logic.

## ++nc vs nc++

In this program, both are identical in effect.

The distinction between pre and post increment only matters
when the value of the expression is used.

Here, the result is discarded.
Either form produces the same behavior.

K&R uses `++nc`, likely to introduce the prefix form.

## Open question

What happens when `nc` exceeds the maximum value of `long`?

Questions:
- Is signed integer overflow defined or undefined in C?
- Do implementations wrap around or trigger errors?
- How would a robust program detect or prevent overflow?

Leaving this open until integer behavior is covered in more detail.

## Connection to later topics

This is the simplest form of an accumulator pattern:

read → process → discard

The same structure appears in:
- word counting
- line counting
- frequency tables
- histograms

The logic changes, but the control flow remains the same.

## Observation

The structure of character counting and line counting is identical.

The only difference is a condition inside the loop body.

That small change transforms the program from counting everything
to selecting specific events.

K&R places them together to show that behavior is defined
by control flow, not by syntax.
