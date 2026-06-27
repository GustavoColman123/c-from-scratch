# Day 12 — Expression Mechanics and Operators

## What I built

A lightweight bridge exercise covering several related sections from K&R Chapter 2:

* 2.7 Type Conversions
* 2.8 Increment and Decrement Operators
* 2.9 Bitwise Operators
* 2.10 Assignment Operators and Expressions
* 2.11 Conditional Expressions

The goal was not to document each operator exhaustively, but to group several expression-level mechanisms into one compact laboratory.

## What changed from Day 11

Day 11 focused on declarations, arithmetic, relational operators, logical operators, and the chained-comparison trap.

Day 12 continues the same theme, but moves deeper into expression mechanics:

> In C, an expression is not just a calculation. It is a typed computation that may perform conversions, select control paths, produce a value, and possibly modify program state through side effects.

## What I built in code

The program is divided into five sections:

* Type conversions
* Prefix and postfix increment
* Bitwise flag manipulation
* Compound assignment operators
* Conditional expressions

The bitwise section was the most relevant to systems programming because it demonstrates the basic operations used to represent and manipulate flags:

```c
flags |= ready;
flags ^= debug;
flags &= ~ready;
```

These patterns will matter later for NucleOS when working with permissions, status bits, interrupt flags, hardware registers, and memory-related metadata.

## What clicked

### Conversions produce values, not mutations

Casting or converting a value usually does not change the original object. It produces a temporary value in another type.

```c
(double) 7 / 3
```

forces floating-point division because the conversion happens before the division.

```c
(double) (7 / 3)
```

is too late because the integer division has already happened.

### Increment operators combine value production and side effects

```c
x = n++;
```

uses the old value of `n`.

```c
x = ++n;
```

uses the new value of `n`.

The important distinction is between the value produced by the expression and the side effect applied to the object.

### Bitwise operators are the language of low-level state

`&`, `|`, `^`, `~`, `<<`, and `>>` are not logical operators. They operate on individual bits.

The central patterns are:

```c
flags |= FLAG;      /* set */
flags &= ~FLAG;     /* clear */
flags ^= FLAG;      /* toggle */
flags & FLAG        /* test */
```

This is directly connected to systems programming.

### Assignment is an expression

An assignment does not only store a value. It also produces a value.

That is why this pattern works:

```c
while ((c = getchar()) != EOF)
```

The assignment stores the character in `c` and also produces the assigned value so it can be compared against `EOF`.

### The conditional operator selects a value

The ternary operator is not just a compressed `if`.

```c
max = (a > b) ? a : b;
```

It is an expression that selects one of two values. It is best used when the goal is value selection, not complex control flow.

## Connection to NucleOS

This day is a bridge toward the C knowledge required for NucleOS.

The most important connection is bit manipulation. Operating systems constantly represent multiple states inside individual bits:

* CPU flags
* permission bits
* page table entries
* interrupt masks
* status registers
* memory-mapped hardware registers

Understanding bitwise operators is not optional for systems programming. It is part of the basic vocabulary.

## Mistakes / confusions

* I had to separate casts from mutation: a cast usually produces a converted value, not a changed object.
* I had to distinguish prefix and postfix increment by the value they produce, not just by the final stored value.
* I had to avoid treating `&` and `&&` as similar operators. One manipulates bits; the other controls logical evaluation.
* I had to remember that assignment operators group expressions and also produce values.
* I had to treat `?:` as value selection, not as a place to hide complex side effects.

## Final reflection

This was a lightweight day, but it clarified an important idea:

C expressions are active structures. They compute values, apply conversions, trigger side effects, manipulate individual bits, and sometimes control whether other expressions execute.

This matters because pointers will build on the same foundation. Once addresses enter the picture, expressions will no longer manipulate only numbers; they will manipulate memory locations.

## Time

Light study session — focused reading, question review, implementation, and documentation.
