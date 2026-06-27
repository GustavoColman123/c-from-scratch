# Expression Mechanics and Operators

## Overview

This exercise is a bridge exercise for K&R Chapter 2.

It covers several small but important sections together:

* 2.7 Type Conversions
* 2.8 Increment and Decrement Operators
* 2.9 Bitwise Operators
* 2.10 Assignment Operators and Expressions
* 2.11 Conditional Expressions

The goal is not to document every operator exhaustively, but to understand how C expressions produce values, perform conversions, trigger side effects, and manipulate bits.

## Concepts Covered

### Type Conversions

* Implicit conversion from `int` to `double`
* Integer division before conversion
* Explicit casts
* Temporary converted values

### Increment and Decrement

* Postfix increment (`n++`)
* Prefix increment (`++n`)
* Difference between produced value and side effect

### Bitwise Operators

* Setting flags with `|`
* Testing flags with `&`
* Toggling flags with `^`
* Clearing flags with `& ~mask`
* Using `unsigned` values for bit manipulation

### Assignment Operators

* Compound assignment (`+=`, `*=`, `>>=`)
* Assignment as an expression
* Updating stored state through operators

### Conditional Expression

* Ternary operator (`?:`)
* Selecting one value from two alternatives
* Using a conditional expression as a function argument

## Program Structure

The program is divided into five independent sections.

### Section 1 — Type Conversions

```c
int integer_value = 7;
double converted_value = integer_value;
double late_conversion = 7 / 3;
double early_conversion = (double) 7 / 3;
```

This section demonstrates that conversions do not change the original object. They produce converted values used in assignments or expressions.

It also shows why:

```c
7 / 3
```

is different from:

```c
(double) 7 / 3
```

The first expression performs integer division before conversion. The second converts one operand before division, forcing floating-point arithmetic.

### Section 2 — Increment and Decrement

```c
x = n++;
x = ++n;
```

This section demonstrates the difference between postfix and prefix increment.

Postfix increment produces the old value and updates the object before the full expression ends.

Prefix increment updates the object first and produces the new value.

### Section 3 — Bitwise Operators

```c
flags |= ready;
flags ^= debug;
flags &= ~ready;
```

This section demonstrates how bit masks are used to set, test, toggle, and clear flags.

These operators are central to systems programming because they allow compact representation of multiple boolean states inside a single integer.

### Section 4 — Assignment Operators

```c
total += 5;
total *= 2;
total >>= 1;
```

This section demonstrates compound assignment operators.

The important rule is that the left-hand side is evaluated only once.

### Section 5 — Conditional Expression

```c
int max = (a > b) ? a : b;
```

This section demonstrates that the conditional operator is an expression.

It selects one of two values and can be used wherever a value is needed, such as inside a function call.

## Example Output

```text
=== Section 1: Type Conversions ===

integer_value                 = 7
converted_value               = 7.00
7 / 3 stored in double         = 2.00
(double) 7 / 3                 = 2.33

=== Section 2: Increment and Decrement ===

after x = n++  -> x = 5, n = 6
after x = ++n  -> x = 6, n = 6

=== Section 3: Bitwise Operators ===

after setting READY and DEBUG  = 5
READY is set                   = 1
ERROR is set                   = 0
after toggling DEBUG           = 1
after clearing READY           = 0

=== Section 4: Assignment Operators ===

initial total                  = 10
after total += 5               = 15
after total *= 2               = 30
after total >>= 1              = 15

=== Section 5: Conditional Expression ===

a                              = 12
b                              = 20
max                            = 20
status                         = large
```

## Key Observations

* A conversion usually produces a temporary converted value; it does not mutate the original object.
* Integer division happens before assignment conversion.
* Prefix and postfix increment differ when the produced value is used.
* Bitwise operators manipulate individual bits, not truth values.
* `unsigned` values are preferred for bit manipulation.
* Compound assignment evaluates the left-hand side only once.
* Assignment expressions produce values.
* The conditional operator selects a value, not a block of statements.

## Lessons Learned

* Expressions in C combine value computation, type rules, side effects, and evaluation constraints.
* Compact syntax is powerful only when the sequencing rules are understood.
* Bitwise operators form the basic language of flags, masks, and low-level state.
* The conditional operator is best used to choose values, not to hide complex control flow.

## Source

Kernighan & Ritchie, Chapter 2:

* 2.7 Type Conversions
* 2.8 Increment and Decrement Operators
* 2.9 Bitwise Operators
* 2.10 Assignment Operators and Expressions
* 2.11 Conditional Expressions
