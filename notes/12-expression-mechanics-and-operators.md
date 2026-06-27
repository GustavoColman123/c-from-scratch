# Notes — Expression Mechanics and Operators

## Source

K&R Chapter 2, sections 2.7–2.11.

This is a bridge note. The goal is not to document every operator exhaustively, but to connect several expression-level mechanisms:

* type conversions
* increment and decrement
* bitwise operators
* assignment operators
* conditional expressions

## 1. Expressions are typed computations

In C, an expression is not only a calculation.

An expression may:

* produce a value
* have a type
* trigger conversions
* modify an object
* evaluate only part of another expression
* manipulate individual bits

Example:

```c
double x = (double) 7 / 3;
```

This expression does more than divide two numbers.

It converts `7` to `double`, converts `3` to `double` through the usual arithmetic conversions, performs floating-point division, and produces a `double` result.

### Key insight

**An expression is a value computation governed by type rules.**

The source code may look like algebra, but C evaluates it according to conversion rules, operator precedence, side effects, and sequencing constraints.

## 2. Type conversions

C converts values between types in several contexts:

* assignment
* arithmetic expressions
* function calls
* casts
* conditional expressions

Example:

```c
int x = 7;
double y = x;
```

`x` remains an `int`.

The value of `x` is converted to `double` to initialize `y`.

The original object is not mutated.

### Integer division before conversion

```c
double a = 7 / 3;
double b = (double) 7 / 3;
```

`a` receives `2.0`.

`7 / 3` is integer division because both operands are integers.

The conversion to `double` happens only after the integer result has already been produced.

`b` receives approximately `2.333333`.

The cast happens before division, so one operand is already `double`.

### Key insight

**A cast usually produces a converted value. It does not rewrite the original object.**

## 3. Increment and decrement

C provides prefix and postfix increment:

```c
++i;
i++;
```

Both increment `i`.

The difference matters when the produced value is used.

### Postfix increment

```c
x = n++;
```

The expression produces the old value of `n`.

The side effect increments `n` before the full expression ends.

If `n` starts as `5`, then:

```text
x = 5
n = 6
```

### Prefix increment

```c
x = ++n;
```

The object is incremented first, and the expression produces the new value.

If `n` starts as `5`, then:

```text
x = 6
n = 6
```

### Key insight

**Increment operators combine value production with side effects.**

This matters later for pointers, where expressions like this become common:

```c
*p++ = *q++;
```

## 4. Bitwise operators

C provides operators for manipulating individual bits:

```text
&   bitwise AND
|   bitwise OR
^   bitwise XOR
~   one's complement
<<  left shift
>>  right shift
```

These operators require integer operands.

They do not operate on truth values. They operate on bit patterns.

### Masking bits

```c
x = x & 077;
```

This keeps the low-order bits selected by the mask and clears the others.

### Setting bits

```c
flags |= READY;
```

OR with `1` forces a bit to `1`.

OR with `0` leaves a bit unchanged.

### Clearing bits

```c
flags &= ~READY;
```

`~READY` creates a mask where the `READY` bit is `0` and the other bits are `1`.

AND with that mask clears only the selected bit.

### Toggling bits

```c
flags ^= DEBUG;
```

XOR with `1` flips a bit.

XOR with `0` leaves a bit unchanged.

### Testing bits

```c
if (flags & READY) {
    /* READY is set */
}
```

This checks whether the selected bit is present.

### Key insight

**Bitwise operators are the language of flags, masks, and compact state.**

## 5. `unsigned` and bit manipulation

Unsigned integers are usually the correct type for raw bit manipulation.

Right shift is especially important:

```c
unsigned x;
x >> 1;
```

For unsigned values, vacated left-side bits are filled with zero.

For signed negative values, right shift is implementation-defined. Many systems perform an arithmetic shift, filling the left side with copies of the sign bit.

### Rule

```text
Use signed integers for arithmetic values.
Use unsigned integers for bit patterns, flags, masks, and shifts.
```

### Key insight

**When the goal is bit manipulation, signedness is part of the program's correctness.**

## 6. Assignment operators

C provides compound assignment operators:

```text
+=  -=  *=  /=  %=  <<=  >>=  &=  ^=  |=
```

Example:

```c
x += y + 1;
```

This is similar to:

```c
x = x + (y + 1);
```

but the left-hand side is evaluated only once.

This matters when the left-hand side is more complex:

```c
array[i++] += value;
```

The target location is computed once.

### Assignment as an expression

An assignment stores a value and also produces a value.

That is why this pattern works:

```c
while ((c = getchar()) != EOF) {
    /* process c */
}
```

The assignment stores the character in `c`.

The assignment expression also produces the assigned value, which is then compared against `EOF`.

### Key insight

**Assignment is not only a statement-like action. It is an expression with a value.**

## 7. Conditional expressions

The conditional operator is written:

```c
condition ? expr1 : expr2
```

It first evaluates the condition.

If the condition is true, only `expr1` is evaluated.

If the condition is false, only `expr2` is evaluated.

Example:

```c
max = (a > b) ? a : b;
```

This selects one value.

It does not select a block of statements.

### Conditional expression as an argument

```c
printf("%s\n", error ? "error" : "ok");
```

This works because the conditional operator produces a value.

### Type of the conditional expression

The two result operands must follow C's type rules.

Example:

```c
int a = 10;
double b = 0.5;

double result = condition ? a : b;
```

The conditional expression has a common type determined by C's conversion rules.

In this case, the expression produces a `double`.

### Key insight

**Use `?:` to select values, not to hide complex control flow.**

## 8. Connection to NucleOS

These sections are directly connected to systems programming.

NucleOS will require precise control over:

* flags
* masks
* permissions
* status bits
* byte-level data
* hardware register layouts
* memory-related metadata

Examples:

```c
flags |= READY;
flags &= ~INTERRUPT_DISABLED;
flags ^= DEBUG_MODE;

if (status & ERROR_BIT) {
    /* handle error */
}
```

These are not advanced tricks.

They are basic systems vocabulary.

### Why this matters

Operating systems often store many boolean states inside a single integer.

A page table entry, device status register, permission field, or interrupt mask may contain several independent meanings packed into different bits.

Understanding `&`, `|`, `^`, `~`, `<<`, and `>>` is required before writing serious kernel code.

## 9. Practical takeaways

1. Conversions produce values; they usually do not mutate the original object.
2. Integer division happens before assignment conversion.
3. Prefix and postfix increment differ when the produced value is used.
4. Bitwise operators manipulate bits, not truth values.
5. Use `unsigned` for raw bit manipulation.
6. Assignment expressions produce values.
7. Compound assignment evaluates the left-hand side only once.
8. Conditional expressions select values.
9. Compact C is safe only when sequencing and type rules are understood.
10. These expression rules prepare the ground for pointers.

## Final observation

Day 12 is a bridge between ordinary expressions and low-level programming.

The important lesson is that C expressions are active structures. They compute values, apply conversions, trigger side effects, manipulate bits, and sometimes determine whether other expressions execute.

This matters because pointers build on the same foundation.

Once addresses enter the picture, expressions will no longer manipulate only numbers. They will manipulate memory locations.
