# Notes — Histograms

## Source

Conceptual extension of K&R Chapter 1.
The book introduces digit frequency counting but does not render it visually.
This extends that program one step further.

## What is a histogram?

A visual representation of a frequency distribution.
Each category gets a bar. The bar's length represents how often that category appeared.

The visual form makes patterns immediately readable —
something a column of numbers does not.

## Why horizontal

Each row is one category. One loop, one line per digit.
Simpler to implement and easier to read at this scale.

## The core operation

A count controls a loop.

`digits[3] = 5` does not print `*****` directly.
It tells an inner loop to run 5 times.
The index is the label. The value is the instruction.

## Scaling

Raw counts as bar lengths break on real input.
A digit that appears 400 times produces a bar that does not fit the screen.

The fix: find the maximum value first, then map every bar proportionally.


scaled = (value * MAX_WIDTH) / max_value


A value equal to the maximum produces exactly `MAX_WIDTH` characters.
Everything else scales down relative to it.

## Conceptual shift

| Stage | Question |
|---|---|
| Counting | How many times did this appear? |
| Histogram | What does that distribution look like? |

The data does not change. The representation does.
This is the first program where the output layer is separate from the logic layer.
