# Word Counting

## Description

Counts the number of transitions from OUT to IN in the input stream.

A word is defined as a sequence of one or more non-whitespace characters.
Whitespace is defined as `' '`, `'\t'`, and `'\n'` for this program.

The program counts transitions into words, not characters inside them.

## Approach

The input is processed as a stream of characters.
Each character is classified as either part of a word (IN)
or a separator (OUT).

A counter is incremented once per word — at the moment the stream
transitions from OUT to IN.

This requires tracking state across iterations.
A single variable, `state`, holds whether the previous character
was inside a word or outside one.

## Why state is necessary

Without state, the program has no memory of previous input.

Since words are defined by transitions, not isolated characters,
state is required to detect boundaries.

The problem is not counting characters.
It is detecting when a new region begins.

## Key insight

The program does not count words directly.
It counts the event of entering a word.

That event occurs exactly once per word,
regardless of its length.

## Operator and logic traps

Placing `words++` outside the `else` block causes it to evaluate
for every character, including whitespace.
A space while `state == OUT` would count a word that does not exist.

Checking only `c != ' '` misses `'\t'` and `'\n'`.
Both are whitespace and must reset state to OUT.
An incomplete whitespace check breaks the transition logic silently.

## Limitation

The definition of whitespace is fixed.

`"hello,world"` returns `1` — the comma is not whitespace,
so both segments are treated as one word.

Changing this behavior requires redefining what counts as a separator.

## Edge cases

The following cases validate the model, not just the implementation.

| Input | words |
|---|---|
| `"hello world"` | 2 |
| `"   hello   world   "` | 2 |
| `"hello"` | 1 |
| `""` | 0 |
| `"   "` | 0 |
| `"hello,world"` | 1 |
| `"hello\tworld\nfoo"` | 3 |
