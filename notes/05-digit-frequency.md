# Notes — Digit Frequency

## Source

K&R Chapter 1, section 1.6.

## What it actually is

A program that classifies characters into categories and accumulates counts.
Not a program that reads numbers. Not a program that stores digits.
It never sees a complete number — only one character at a time,
mapped immediately to a counter or discarded into a catch-all bucket.

## The definition of digit used here

Any character in the range `'0'` through `'9'`.
The range is not assumed — it is guaranteed by the C standard,
which requires digit characters to be contiguous and ordered.

This definition is representational, not mathematical.
The program does not know what a number means.
It knows what digit characters look like, and maps them to indices.

## Why an array is the core of this program

The previous programs used scalar counters.
One variable, one thing counted.

This program needs ten counters that share structure.
An array makes that structure explicit — and makes the index meaningful.

`digits[3]` is not an arbitrary memory location.
It is "the frequency of the digit 3."
The index carries semantic weight that a set of ten separate variables would not.

## Why mapping matters more than storage

The key operation is not incrementing a counter.
It is converting a character to a valid index before touching the array.
if (c >= '0' && c <= '9')
    ++digits[c - '0'];

The guard and the subtraction are inseparable.
The guard guarantees the index is in `[0, 9]`.
The subtraction produces that index from the character's encoding.

Without the guard, the subtraction still runs — but on characters
it was never meant to handle. The result is an out-of-bounds access
with no warning from the compiler.

## Why `digits[10]` and not `freq[256]`

`freq[256]` would work mechanically.
But it solves a different problem — arbitrary character frequency
across the full ASCII range.

Here the domain is fixed: exactly ten digits, each with a logical index.
Using `freq` would make the index meaningless.
`freq[51]` and `digits[3]` hold the same value,
but only one of them communicates what it represents.

The array size is a design decision, not a technical one.

## Why initialization is not optional

`digits` is a local array. Local variables in C are not zeroed automatically.
They contain whatever bytes were last written to that memory.

Skipping the initialization loop does not produce a compile error.
It produces counters that start at garbage values —
and a program that appears to work while producing wrong output.

## Open question

The whitespace check is hardcoded to `' '`, `'\t'`, and `'\n'`.
The digit check is hardcoded to a character range.

I do not know yet if C has a standard way to classify characters —
something like `isdigit()` or `isspace()` — that handles edge cases
automatically and works correctly regardless of encoding.
If it exists, both checks in this program probably should use it.
Leaving this open.

## Connection to later topics

This is the first program that uses an array as a map, not just as storage.
My guess is that lookup tables, frequency analysis, and symbol tables
are all built on the same idea at a larger scale —
a structure where the index encodes meaning, not just position.

If that is right, this is the first instance of a pattern
that appears throughout systems programming.

## Observation

K&R could have used ten separate variables.
They chose an array, defined its size as a meaningful constant,
and let the index do the semantic work.

That decision teaches something beyond the program itself:
when a set of values shares structure, make the structure explicit.
A scalar is for one thing. An array is for one thing, ten times —
and the difference between position 3 and position 7 should mean something.

## Hidden constraint

Array access is only valid if the index is guaranteed to be within bounds.

This program enforces that constraint explicitly before every access.

In C, this guarantee is the programmer’s responsibility.
