# Notes — Functions

## Source

K&R Chapter 1, section 1.7.

## What a function actually is

A named, reusable unit of computation.
It receives input, performs a task, and optionally returns a value.
The caller does not need to know how it works, only what to give it and what to expect back.

## Prototypes as contracts

A prototype declares the terms before the implementation exists.

int power(int base, int n);

This tells the compiler: `power` takes two `int` arguments and returns an `int`.
Every call is checked against that contract.
If the types do not match, the compiler catches it before the program runs.

Without a prototype, the compiler assumes. Assumptions in C are silent and dangerous.

## Parameters vs arguments

These are not the same thing.

**Parameters** are the variables in the function definition.
`int base, int n` — they exist inside `power`.

**Arguments** are the values passed at the call site.
`power(2, i)` — `2` and `i` exist in `main`.

At the moment of the call, arguments are copied into parameters.
After that, the function works entirely with its own copies.
Modifying `base` inside `power` does not affect anything in `main`.

## Scope and lifetime of variables

A local variable exists only inside the function where it is declared.
When the function returns, the variable is gone.

The `i` in `power` and the `i` in `main` are two separate variables.
They happen to share a name. They do not share memory.

This is not a coincidence of implementation — it is a guarantee of the language.
Scope is what makes functions composable without interference.

## Pass-by-value

C passes arguments by value. Always.

The function receives a copy, not the original.
This means a function cannot modify a variable in the caller
unless it is given a way to reach it directly — which requires pointers.

For now: what happens inside a function stays inside the function.

## Why modularity matters

A monolithic program is a sequence of steps.
A modular program is a composition of named computations.

Modularity does not just organize code — it changes how problems are solved.
A complex task becomes a set of smaller, independently testable units.
Each function can be reasoned about in isolation.

## Historical note — old-style K&R syntax

Before ANSI C (C89), functions were defined without types in the parameter list:
power(base, n)
int base, n;
{
    ...
}

The compiler had no way to verify argument types at the call site.
Type mismatches produced undefined behavior with no warning.

ANSI C introduced typed parameters and prototypes.
The old style still compiles in some environments but should never be used.
It exists in K&R for historical context, not as a model to follow.

## Open question

A function can only return one value.
If a computation needs to produce multiple results,
the return value alone is not enough.

I do not know yet how C handles this.
My guess is it involves pointers — giving the function a way to write
directly into the caller's variables.
Leaving this open.
