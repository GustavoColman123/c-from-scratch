# Day 04 — Word Counting

## What I built

Implemented a program that counts transitions into words from stdin until EOF.

A word is a sequence of non-whitespace characters.
The program counts entries into words, not the characters inside them.

## What changed from Day 03

Day 03 filtered on a single value (`'\n'`).

This program introduces classification and state.
Each character is evaluated in context, not isolation.

The loop now maintains memory across iterations.

## What I had to define before writing code

What a word is — in terms the program can evaluate.
Which characters are IN and which are OUT.
When exactly to increment.
What state to start in.

This is the first program where the problem must be defined
before it can be implemented.

Without a precise definition, the code has no correct form.

## Model

The program is based on a two-state model:

- OUT → not inside a word
- IN  → inside a word

Words are detected as transitions from OUT to IN.

This model determines the structure of the entire program.

## Key difference in thinking

Day 03: the loop asks "what is this character?"
Day 04: the loop asks "what is this character, and what was the previous state?"

That second question introduces temporal context.

## What clicked

The increment does not happen when a word ends.
It happens when a word begins.

This means EOF does not need special handling.
By the time the stream ends, every word has already been counted
at the moment it started.

## Mistakes / confusions

- Placing `words++` outside the `else` block caused it to execute
  for whitespace while `state == OUT`.
  This violated the transition model and produced false positives.

- Using `c != ' '` missed `'\t'` and `'\n'`.
  The classification was incomplete and broke the logic on real inputs.

- Initializing `state` incorrectly would miscount the first word.
  OUT is required to correctly detect the first transition.

## Time

45 min
