# Day 05 — Digit Frequency

## What I built

A program that classifies input characters into three categories:
digits, whitespace, and other.

Digits are counted individually using an array of size 10.
Whitespace and other characters each have a single counter.
## What changed from Day 04

Day 04 relied on state transitions to detect word boundaries.

This program does not track explicit state transitions like Day 04.

Instead, state is implicit in the accumulated counts.
Each character contributes directly to a category.r.


## Key difference in thinking

Before: detect events (entering a word)  
Now: classify input and accumulate counts

The logic shifts from detecting transitions
to assigning every input to a category.

This removes temporal dependency (previous character)
and replaces it with direct classification.


## What I had to define before writing code

- Which categories exist
- Which characters belong to each category
- How to map digit characters to array indices safely
- Why `digits[10]` and not `freq[256]`
- How to guarantee that all array accesses are safe

## What clicked

The array index represents meaning.

`digits[3]` is not just a memory location —
it is a semantic mapping: "count of digit 3".

This separates representation (array)
from interpretation (what the index means).

---

## Mistakes / confusions

- - Initially treated array initialization as optional
  → led to undefined behavior, not just incorrect values

- Had to reason explicitly about why the range guard
  must come before indexing

- Realized classification must be exhaustive:
  every character must belong to exactly one category

---

## Time

45 min
