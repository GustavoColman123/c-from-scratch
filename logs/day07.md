# Day 07 — Functions: power(base, n)

## What I built

Implemented the `power` function from K&R Chapter 1.
Computes integer exponentiation through iterative multiplication.

## What changed from Day 06

Days 01–06 were monolithic — all logic lived inside `main`.

This is the first program where a computation is extracted into its own unit.
`main` does not know how `power` works. It only knows what to pass and what to expect back.

## What I had to define before writing code

- What the function receives and what it returns
- Where the prototype goes and why it must appear before `main`
- Why local variables inside `power` do not affect `main`

## What clicked

A function is a contract.

The prototype declares the terms: two `int` arguments, one `int` returned.
The implementation fulfills them.
The caller does not need to read the implementation to use it correctly.

## Key difference in thinking

Before: the program is a sequence of steps.
Now: the program is a composition of named computations.

`power(2, i)` inside `printf` is not a special case — it is an expression.
Any function that returns a value can appear anywhere a value is expected.

## Mistakes / confusions

Parameters and arguments are not the same thing.
`base` and `n` are parameters — they exist inside the function.
`2` and `i` are arguments, they exist at the call site.
The distinction matters when reasoning about scope and pass-by-value.

## Time

30–45 min
