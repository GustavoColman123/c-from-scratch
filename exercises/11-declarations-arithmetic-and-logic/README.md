# Declarations, Arithmetic Operators, and Logical Expressions

## Overview

This exercise explores three core ideas from K&R Chapter 2:

- Variable declarations and initialization
- Arithmetic operators and integer division
- Relational and logical expressions

The goal is not to memorize syntax, but to understand how the compiler interprets declarations, evaluates expressions, and converts values during computation.

## Concepts Covered

### Declarations

- Variable declarations
- Initialization
- Automatic vs static storage duration
- `const` qualifier
- Declaration versus definition

### Arithmetic Operators

- Addition, subtraction, multiplication
- Integer division
- Modulus operator (`%`)
- Operator precedence
- Overflow considerations

### Relational and Logical Operators

- Relational operators (`<`, `>`, `<=`, `>=`)
- Equality operators (`==`, `!=`)
- Logical operators (`&&`, `||`, `!`)
- Short-circuit evaluation
- Expression precedence

## Program Structure

The program is divided into five independent sections.

### Section 1 — Declarations and Initialization

```c
int automatic;
int initialized = 42;
static int persistent;
const int constant = 10;
```

`automatic` is never read. Reading an uninitialized local variable is undefined behavior.
The program only prints what is safe to print:

```text
initialized = 42
persistent  = 0
constant    = 10
```

`persistent` is zero because static variables are zero-initialized before `main`.
`constant` is a typed object in memory — not a macro substitution.

### Section 2 — Integer Division and Modulus

```c
7 / 3
-7 / 3
7 % 3
-7 % 3
```

Also demonstrates the invariant that must always hold:

```c
(a / b) * b + (a % b) == a
```

Integer division truncates toward zero. The sign of `%` follows the dividend, not the divisor.

### Section 3 — Relational Operators

```c
5 > 3
5 < 3
5 == 5
5 != 5
```

Expected output:

```text
1
0
1
0
```

Relational expressions evaluate to `1` (true) or `0` (false). C has no boolean type here — only integers.

### Section 4 — Logical Operators

```c
int valid = 1;
int error = 0;

valid && error
valid || error
!valid
!error
```

Demonstrates short-circuit behavior: the right operand of `&&` and `||` is only evaluated
if the left operand does not already determine the result.

### Section 5 — The Chained Comparison Trap

```c
int x = 20;

printf("%d\n", 0 < x < 10);
printf("%d\n", (0 < x) && (x < 10));
```

Expected output:

```text
0 < x < 10         => 1
(0 < x) && (x < 10)=> 0
```

`0 < x < 10` does not work like mathematics. C evaluates it as `(0 < x) < 10`,
which reduces to `1 < 10` — always true. The correct form requires two explicit comparisons joined by `&&`.

## Example Output

```text
initialized = 42
persistent  = 0
constant    = 10

7 / 3   =  2
-7 / 3  = -2
7 % 3   =  1
-7 % 3  = -1
(a/b)*b + (a%b) = a => verified

5 > 3   = 1
5 < 3   = 0
5 == 5  = 1
5 != 5  = 0

valid && error = 0
valid || error = 1
!valid         = 0
!error         = 1

0 < x < 10         => 1
(0 < x) && (x < 10)=> 0
```

## Key Observations

- A declaration specifies both a type and a name.
- Local variables are not automatically initialized.
- Static variables are initialized before `main`.
- Integer division discards the fractional part.
- `%` works only with integer operands.
- Relational expressions evaluate to `0` or `1`.
- Logical operators use short-circuit evaluation.
- Expression meaning depends heavily on precedence rules.-

## Lessons Learned

- A type describes how bits are interpreted.
- Initialization depends on storage duration.
- Arithmetic in C follows machine-oriented rules, not pure mathematics.
- Boolean logic in C is represented with integers.
- Parentheses improve correctness and readability.
- Understanding evaluation order prevents subtle bugs.


## Related Notes

- Notes — Declarations
- Notes — Arithmetic Operators
- Notes — Relational and Logical Operators

## Source

Kernighan & Ritchie, Chapter 2:

- 2.4 Declarations
- 2.5 Arithmetic Operators
- 2.6 Relational and Logical Operators
