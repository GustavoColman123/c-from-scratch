# Day 03 — Line Counting

## What I built

A program that counts newline characters from stdin until EOF.

A line is delimited by `'\n'`. The program counts delimiters, not logical lines.

## What changed from Day 02

Day 02 asked: is there still input?

This program asks: what is this character?

The loop structure is the same. The question inside it is different.

## What I had to define before writing code

- What counts as a line boundary
- Whether the counter lives inside or outside the loop
- Why `c` must be `int`, not `char`

## Key difference in thinking

Day 02: the loop processes input.
Day 03: the loop inspects input.

Inspection requires storing the character. That storage is the only new thing.

## What clicked

`'\n'` is not special syntax. It is the integer value `10`.
The `if` is comparing two integers. The character notation exists for the reader, not the compiler.

Once that was clear, the condition stopped feeling like magic.

## Mistakes / confusions

Wrote `char c` out of habit. Compiled fine.
Remembered the EOF problem from earlier and changed it to `int` before testing.

Forgot the parentheses in the while condition.
`c` was getting assigned `0` or `1` instead of the actual character.
Counted zero lines on every input. Took longer than expected to spot.

Operator precedence with `=` and `!=` in the same expression still does not feel automatic.
The rule is understood. The instinct is not there yet.

## Limitation

If the input does not end with `'\n'`, the last line is not counted.
The program counts delimiters, not logical lines.
A trailing segment without a delimiter is silently ignored.

## Time

40 min
