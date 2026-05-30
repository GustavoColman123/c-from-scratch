# Notes — Word Counting

## Source

K&R Chapter 1, section 1.5.4.

## What it actually is

A program that detects transitions from OUT to IN in the input stream.

It does not read words.
It does not store words.
It never sees a complete word — only one character at a time,
with knowledge of the previous state.

## The definition of word used here

A sequence of one or more non-whitespace characters.

Whitespace is defined as `' '`, `'\t'`, and `'\n'` for this program.
Everything else is part of a word — letters, digits, punctuation, symbols.

This definition is operational, not linguistic.
The program does not know what a word means.
It knows what whitespace is, and infers boundaries from its absence.

## Why state is the core of this program

Previous programs were stateless within the loop.
Each character was processed independently.

This program cannot do that.

The decision to increment depends not only on the current character,
but on the previous state of the stream.

One bit of state is sufficient: IN or OUT.

---

## Why IN and OUT are defined as constants

The program works with `1` and `0`.
But `state == OUT` communicates intent.
`state == 0` does not.

This is the first time where naming values improves clarity
more than it affects execution.

## The transition model

The program is event-driven.

It does not count words as objects.
It counts transitions into words.

Each word produces exactly one event,
independent of its length.

## Open question

The whitespace definition is hardcoded.

I do not know yet if C has a standard way to classify characters —
something like `isspace()` — that would handle edge cases
like `'\r'` or other non-printing characters automatically.

If it exists, this program should likely use it.
Leaving this open.

## Connection to later topics

This is essentially a two-state finite machine.

More complex systems (parsers, tokenizers)
extend this idea with more states and transitions.

If that is correct, this is the conceptual foundation
for a large class of programs.

## Observation

K&R could have introduced `state` without naming it.

They chose to name it, define constants for its values,
and make the model explicit.

That decision teaches something beyond the program itself:
when a variable represents a condition, name the condition.

The code is for the compiler.
The names are for the person reading it.
