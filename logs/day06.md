# Day 06 — Digit Histogram

## What I built

A program that reads characters from stdin, counts digit frequencies,
and renders the distribution as a horizontal histogram.

An extension of Day 05 — same classification logic, new output layer.

## What changed from Day 05

Day 05 asked: how many times did each digit appear?
Day 06 asks: what does that distribution look like?

The counting logic is identical. What changed is what happens after the loop ends.

## What I had to define before writing code

- How to convert a count into a visual length
- What to do when counts are large
- How to iterate over the array to produce one bar per digit

---

## Key difference in thinking

Day 05: accumulate, then print numbers.
Day 06: accumulate, then drive a second loop with those numbers.

The value at `digits[3]` stops being an answer and becomes an instruction — repeat this action `n` times.

## What clicked

A number can control a loop.

`digits[3] = 5` does not print `*****` directly.
It tells a loop to run 5 times, each time printing one `*`.
The index is the label. The value is the loop bound.

For large counts, the value needs to be scaled first.
Find the maximum across all digits, then map each value
proportionally to a fixed column width.

## Mistakes / confusions

Forgot to find the maximum before the render loop.
Bars were proportional to nothing, just raw counts as lengths.
Worked fine on small inputs, broke visually on anything real.

Off-by-one on the scaling formula.
A value equal to the maximum should produce exactly the full bar width,
not one short. Took a test case at the boundary to catch it.

## Time

1 hour
