# Problems — Day 11 (Declarations, Arithmetic Operators, and Logical Expressions)

## Problem 1 — Assuming local variables start at zero

**What I wrote**

```c
int x;
printf("%d\n", x);
```

**What I thought**

Coming from higher-level environments, I expected the runtime to clear the memory grid for safety. I assumed `x` would naturally default to `0`.

**What actually happens**

`x` has an indeterminate value. Reading it before a formal write is Undefined Behavior (UB). A particular build or run may appear to print bytes previously present in stack storage, but the language guarantees absolutely no result, and the compiler is free to optimize the entire branch away.

**Fix**

Never read an automatic variable before a formal write. Initialize it during declaration or assign a value immediately.

```c
int x = 0;
printf("%d\n", x);
```

## Problem 2 — Assuming `static` only changes visibility

**What I wrote**

```c
static int counter;
```

**What I thought**

I thought `static` at the local level was just a scope-limiting keyword to keep things private, acting like an access modifier without shifting how the variable physically lives.

**What actually happens**

It changes the object's storage duration from automatic to static, meaning its lifetime spans the entire execution of the program. Toolchains commonly place zero-initialized static objects in the `.bss` segment and nonzero-initialized ones in `.data`. This introduces shared, persistent state. Such state can break function reentrancy and may require synchronization when accessed concurrently.

**Fix**

Recognize that `static` alters storage duration and persistence, not just visibility. If you only want an isolated, temporary counter per function call, drop the keyword.

```c
int counter = 0; /* Fresh state on every invocation */
```

## Problem 3 — Treating `const` like `#define`

**What I wrote**

```c
const int limit = 10;
int array[limit]; /* Complains under strict ISO C90 */
```

**What I thought**

I assumed `const` was just a modern, type-safe way to create a compile-time constant alias, identical to `#define` but cleaner.

**What actually happens**

`const` declares a typed object in the C language. While it may have storage and an address, it is not an integer constant expression. In strict ISO C90, you cannot use it to size static arrays. Note: while C99 introduced Variable Length Arrays for automatic storage, it still does not convert `limit` into a compile-time constant expression.

**Fix**

Use `#define` or `enum` for structural macro sizes and strict compile-time constant expressions. Use `const` to enforce read-only access paths on variables or pointers.

```c
#define LIMIT 10
int array[LIMIT];
```

## Problem 4 — Assuming integer division behaves like real-number division

**What I wrote**

```c
double ratio = 7 / 3;
printf("%f\n", ratio);
```

**What I thought**

Since the destination variable `ratio` is a `double`, I expected C to realize I wanted fractional results and evaluate the expression to `2.333333`.

**What actually happens**

C evaluates expressions based on the operands, without checking where the result will be stored. Since `7` and `3` are both integer literals, C performs integer division, truncates the fractional part toward zero, gets `2`, and then converts that plain `2` into `2.000000` to place it in the `double`.

**Fix**

Force at least one operand to be a floating-point literal so the calculation uses floating-point arithmetic.

```c
double ratio = 7.0 / 3;
printf("%f\n", ratio); /* Correctly yields 2.333333 */
```

## Problem 5 — Treating `%` as universal mathematical modulo

**What I wrote**

```c
printf("%d\n", -7 % 3);
```

**What I thought**

I thought `%` operated like standard clock arithmetic (mathematical modulo), where the result is always non-negative. I expected `-7 % 3` to wrap around and output `2`.

**What actually happens**

In C, `%` computes the remainder of integer division, not the mathematical modulo. Because integer division truncates toward zero (`-7 / 3` becomes `-2`), the remainder must preserve the sign of the dividend to make the identity `(a / b) * b + (a % b) == a` hold true. The expression evaluates to `-1`.

**Fix**

If you require a strictly positive mathematical modulo for wrapping indices, manually correct the negative output. This specific correction assumes a positive divisor.

```c
int rem = -7 % 3;
if (rem < 0)
    rem += 3; /* Yields 2 for a positive modulus */
```

## Problem 6 — Forgetting that division by zero is undefined behavior

**What I wrote**

```c
int result = a / b;
```

**What I thought**

I figured that if `b` happened to be `0`, C would safely return `0`, set an error flag, or at worst, the compiler would catch it.

**What actually happens**

Division or remainder by zero is Undefined Behavior. A typical hosted implementation may trap at runtime, often triggering a low-level OS signal such as `SIGFPE`, but C does not guarantee a particular diagnostic, signal, or recovery path. The language relinquishes all behavioral guarantees.

**Fix**

Always guard divisions or remainder operations with a defensive logical check if the divisor originates from runtime input.

```c
int result = (b != 0) ? (a / b) : 0;
```

## Problem 7 — Confusing precedence with evaluation order

**What I wrote**

```c
int result = f() + g() * h();
```

**What I thought**

Because operator precedence dictates that multiplication happens before addition, I assumed the CPU must execute `g()` and `h()` before it executes `f()`.

**What actually happens**

Precedence only tells the compiler how to group the expression tree: `f() + (g() * h())`. It does not dictate the chronological sequence of execution for subexpressions. If the functions have side effects, relying on an assumed call order can produce different observable behavior. If unsequenced side effects modify the same scalar object, the expression may have undefined behavior.

**Fix**

Do not rely on precedence to determine execution timing. If sequence matters because of side effects, break the steps into separate lines.

```c
int val1 = f();
int val2 = g();
int val3 = h();

int result = val1 + (val2 * val3);
```

## Problem 8 — Treating short-circuit as a performance optimization only

**What I wrote**

```c
if ((c = getchar()) != EOF && i < lim - 1)
```

**What I thought**

I flipped the layout of the K&R example because I thought checking a variable condition or fetching a character could be written in any sequence, viewing short-circuiting as just a subtle trick to skip useless work.

**What actually happens**

Because of the flipped order, `getchar()` executes first. If the buffer is already full (`i >= lim - 1`), the program consumes an extra character from the input stream and only then discovers that it has nowhere to store it. The short-circuit rule is a control-flow guarantee.

**Fix**

Place guards to the left of a risky expression when the right-hand side is only valid if the guard succeeds.

```c
if (i < lim - 1 && (c = getchar()) != EOF)
```

## Problem 9 — Writing mathematical chained comparisons in C

**What I wrote**

```c
0 < x < 10
```

**What I thought**

I expected C to process this like standard mathematical notation, checking whether `x` falls between `0` and `10`.

**What actually happens**

C parses it as `(0 < x) < 10`. The inner expression resolves to either integer `0` (false) or integer `1` (true). Then the outer expression compares that value against `10`. Since both `0 < 10` and `1 < 10` are true, the complete expression always returns `1`, allowing invalid out-of-range values through.

**Fix**

Split the relational constraints explicitly using `&&`.

```c
0 < x && x < 10
```

## Problem 10 — Confusing `=` with `==`

**What I wrote**

```c
if (x = 10) {
    /* Always executes */
}
```

**What I thought**

I thought the compiler would catch my typo, realize I wanted a comparison, or reject an assignment inside a conditional guard.

**What actually happens**

In C, an assignment expression such as `x = 10` has a value: the value being assigned. The condition interprets the nonzero integer `10` as true. The branch therefore executes every time while silently rewriting `x`.

**Fix**

Use `==` for comparisons. Compiler warnings such as `-Wall` and `-Wparentheses` can also catch suspicious assignments.

```c
if (x == 10) {
    /* Executes only when x equals 10 */
}
```

## Hidden invariants

* Automatic variables must be initialized before every read.
* Divisors must be nonzero before `/` or `%`.
* Conditions with side effects rely on left-to-right short-circuit evaluation.
* Range checks must compare the same variable explicitly on both sides.
* Parentheses may clarify grouping, but they do not always define evaluation order.

## Summary of corrections

| Problem | Correct model |
|---|---|
| Local variables | Storage exists, but value may be indeterminate |
| `static` | Affects storage duration and persistent state |
| `const` vs `#define` | Typed object versus textual substitution |
| Integer division | Truncates toward zero |
| `%` | Remainder follows C division rules |
| Division by zero | Undefined behavior |
| Precedence | Determines grouping |
| Evaluation order | Determines execution timing |
| Short-circuit | Controls whether later expressions run |
| Chained comparison | Must be written with `&&` |
| `=` vs `==` | Assignment is not comparison |

## Final observation

The dangerous mistakes in these sections are rarely syntax errors. Most of them compile perfectly. The real problem is that the program expresses a completely different rule than the programmer intended. C can execute buggy logic efficiently when the code matches the grammar rules. C gives exact rules at the abstract-machine level; it does not verify that your reasoning matches your intent.
