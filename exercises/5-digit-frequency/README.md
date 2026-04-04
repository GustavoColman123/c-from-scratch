# Digit Frequency (Character Classification)

## Description

Reads characters from stdin until EOF and classifies each one into three categories: digits, whitespace, and other.

Digit frequencies are stored in an array indexed by the digit's numeric value. Whitespace and non-digit characters are tracked with individual counters.


## Context

This program introduces arrays as a way to represent multiple counters
indexed by meaning.

It marks the transition from state-based logic (previous exercises)
to classification and data organization.

## Approach

Input is processed as a character stream using `getchar()` inside a `while` loop.

Each character is classified into exactly one category.
The classification is total and mutually exclusive.

The digit check uses a range guard before indexing the array:

if (c >= '0' && c <= '9')
    ++digits[c - '0'];

This guarantees the index stays within [0, 9] before the array is accessed.

Why digits[10], not freq[256]

freq[256] would work, but it solves a different problem — arbitrary character frequency. Here the domain is known: exactly 10 digits, each with a logical index.

digits[3] means "how many times did the digit 3 appear." freq[51] means the same thing, but requires knowing that '3' is ASCII 51. The mapping approach makes the index meaningful.

This is not a memory optimization — it is a modeling decision.

## Key Insight

c - '0' is not an ASCII assumption.

It relies on a guarantee from the C standard:
digit characters are contiguous and ordered in the execution character set.

The subtraction normalizes that range into [0, 9],
independent of the underlying encoding.
Initialization

digits is a local array.

Local variables in C are not automatically initialized.
Reading from an uninitialized array results in undefined behavior,
not just incorrect values.

Explicit initialization is required.

## Output

Only digits with non-zero frequency are printed.

This improves readability but hides zero-frequency entries,
which may be useful for debugging or verification.
