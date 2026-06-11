# Notes — Declarations, Arithmetic Operators, and Logical Expressions

## Source

K&R Chapter 2, sections 2.4–2.6.

## 1. Declarations are more than syntax

A declaration does not merely introduce a name. It tells the compiler:

* what type an object has
* how much storage is needed
* how the stored bits must be interpreted
* whether initialization occurs
* how long the storage remains valid

Example:

```c
int x;
int y = 0;
static int z;
const int limit = 10;
```

These declarations look similar, but they create objects with different initialization rules and storage behavior.

### Questions to answer

**What is the initial value of each object?**

* `x` has an indeterminate value.
* `y` starts explicitly at `0`.
* `z` is automatically initialized to `0` because it has static storage duration.
* `limit` is initialized to `10`.

**When is initialization performed?**

* For `y`, initialization occurs each time execution reaches its declaration.
* For `z`, initialization occurs once before `main` begins execution.
* For `limit`, initialization follows the storage duration of the object. If declared locally, it is initialized when execution reaches the declaration. If declared at file scope, it is initialized before `main`.

**Which reads are safe?**

* Reading `y`, `z`, and `limit` is safe.
* Reading `x` before assigning a value is undefined behavior.

**Which declarations create persistent state?**

* `z` creates persistent state because it has static storage duration.
* `x` and `y` disappear when their enclosing block ends.
* `limit` is only persistent if declared at file scope.

## 2. Automatic variables

```c
int x;
```

An automatic local variable without an initializer has an indeterminate value.

Reading it before assignment is undefined behavior.

### Important distinction

* The object exists.
* Its storage exists.
* Its value is not safe to use.

The memory may contain bytes left by previous activity on the stack, but the language provides no guarantee about their meaning.

### Example

```c
int x;

/* UB: x has not been initialized */
printf("%d\n", x);
```

### Key insight

**Storage allocation is not initialization.**

Reserving memory and assigning a valid value are separate operations.

## 3. Static variables

```c
static int persistent;
```

A static object has static storage duration.

Its lifetime is the entire execution of the program.

If no initializer is provided, it is zero-initialized before `main`.

### Example

```c
static int persistent;

printf("%d\n", persistent); /* guaranteed: 0 */
```

### Important consequence

A local static variable preserves its value between function calls.

This can be useful, but it also introduces hidden state.

### Systems perspective

Static variables are typically stored in the program's `.bss` or `.data` sections.

Because the storage is shared across calls, functions that depend on static state are not naturally reentrant. Multiple threads may require synchronization when accessing the same static object.

## 4. `const` objects and macros

```c
const int limit = 10;
#define LIMIT 10
```

These are not equivalent.

### `const`

* participates in the type system
* creates an object
* has a type
* may occupy storage
* may have an address
* restricts modification through that access path

### `#define`

* is handled by the preprocessor
* performs textual substitution
* has no type
* has no storage
* has no address
* disappears before compilation

### Key distinction

`const` belongs to the language and is understood by the compiler.

`#define` belongs to preprocessing and performs textual replacement before compilation begins.

## 5. Arithmetic operators

The arithmetic operators introduced here are:

```c
+  -  *  /  %
```

Their behavior depends on operand types.

### Integer division

```c
7 / 3   /* 2 */
-7 / 3  /* -2 */
```

Integer division truncates toward zero.

It does not round.

### Modulus

```c
7 % 3   /* 1 */
-7 % 3  /* -1 */
```

For integer operands:

```c
(a / b) * b + (a % b) == a
```

provided that:

```c
b != 0
```

### Questions to answer

**Why does `%` follow the sign of the dividend?**

Because the language preserves the identity:

```c
(a / b) * b + (a % b) == a
```

If `-7 / 3` produces `-2`, then `-7 % 3` must be `-1` for the equation to remain true.

**Why is division by zero undefined behavior?**

At the hardware level, processors typically raise a CPU exception when division by zero occurs. C does not attempt to define recovery behavior and instead leaves the operation undefined.

**Why does `7 / 3` not produce `2.333...`?**

Because both operands are integers. In C, the operation is performed according to the operand types before the result is considered. Integer divided by integer produces an integer result.

## 6. Relational operators

Relational operators:


```c
<  <=  >  >=
```

Equality operators:

```c
==  !=
```

These expressions produce integer values:

```c
1 /* true */
0 /* false */
```

### Example

```c
5 > 3   /* 1 */
5 < 3   /* 0 */
```

### Important note

Traditional C code commonly uses integers as boolean values.

Modern C also provides `_Bool` and `<stdbool.h>`, but relational expressions still evaluate to `0` or `1`.

The CPU itself operates on status flags such as zero, sign, carry, and overflow. C translates those conditions into ordinary integer values.

## 7. Logical operators

Logical operators:

```c
&&  ||  !
```

They interpret:

* `0` as false
* any nonzero value as true

Examples:

```c
1 && 0  /* 0 */
1 || 0  /* 1 */
!1      /* 0 */
!0      /* 1 */
```

Nonzero includes values such as `-1`, `42`, or a valid pointer address.

## 8. Short-circuit evaluation

`&&` and `||` evaluate left to right.

The right operand is only evaluated when necessary.

### Example

```c
i < lim - 1 && (c = getchar()) != '\n' && c != EOF
```

This order matters.

### Why it matters

The condition first verifies that space remains in the buffer.

Only then does it consume input using `getchar()`.

If the order changes, the program may read characters that it cannot store, resulting in incorrect behavior or lost input.

### Key insight

**Short-circuit evaluation is not merely an optimization. It is part of the program's control flow.**

The operators act as built-in guards that determine whether later expressions should execute at all.

## 9. Operator precedence

### Example

```c
x + y * z < limit && valid != 0
```

This groups as:

```c
((x + (y * z)) < limit) && (valid != 0)
```

Precedence determines grouping.

It does not determine every aspect of evaluation order.

### Important distinction

**Precedence**

Determines how the parser builds the expression tree.

**Evaluation order**

Determines when subexpressions execute.

For example:

```c
f() + g()
```

does not guarantee which function executes first.

**Short-circuit**

Determines whether some branches of the expression tree execute at all.

These concepts are related, but they are not the same.

## 10. The chained comparison trap

This expression is wrong:

```c
0 < x < 10
```

C parses it as:

```c
(0 < x) < 10
```

The first comparison produces either `0` or `1`.

Both values are less than `10`.

Therefore the complete expression is always true, regardless of the value of `x`.

### Correct version

```c
0 < x && x < 10
```

### Key insight

**Valid C syntax can still encode invalid reasoning.**

The compiler accepts the expression because it is grammatically correct.

Understanding the meaning remains the programmer's responsibility.

## 11. What the chapter demonstrates

The program demonstrates:

* safe and unsafe initialization states
* static zero-initialization
* typed `const` objects versus textual substitution
* integer division and modulus
* relational results as literal `0` and `1`
* logical operators as control-flow selectors
* short-circuit control flow
* precedence versus evaluation order
* the chained comparison trap

## 12. Practical takeaways

1. Never read automatic variables before assigning a value.
2. Treat static state as persistent state.
3. Do not confuse `const` with macros.
4. Remember that integer division truncates toward zero.
5. Use `%` only with integer operands and nonzero divisors.
6. Use parentheses when they improve readability.
7. Rely on short-circuit evaluation deliberately.
8. Never write mathematical chained comparisons in C.

## Final observation

The difficult part of these sections is not syntax.

The difficult part is learning that syntactically valid expressions may still represent the wrong mental model.

C provides precise mechanical rules.

It does not verify that your reasoning matches your intent.

Understanding the distinction is more important than memorizing the operators themselves.
