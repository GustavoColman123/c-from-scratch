# Problems — Day 11 (Declarations, Arithmetic Operators, and Logical Expressions)
## Problem 1 — Assuming local variables start at zero
**What I wrote**
```c
int x;
printf("%d\n", x);

```
**What I thought**
Coming from higher-level environments, I expected the runtime to clear the memory grid for safety. I assumed x would naturally default to 0.
**What actually happens**
x has an indeterminate value. Reading it before a formal write is Undefined Behavior (UB). A particular build or run may appear to print bytes previously present in stack storage, but the language guarantees absolutely no result, and the compiler is free to optimize the entire branch away.
**Fix**
Never read an automatic variable before a formal write. Initialize it during declaration or assign a value immediately.
```c
int x = 0;
printf("%d\n", x);

```
## Problem 2 — Assuming static only changes visibility
**What I wrote**
```c
static int counter;

```
**What I thought**
I thought static at the local level was just a scope-limiting keyword to keep things private, acting like an access modifier without shifting how the variable physically lives.
**What actually happens**
It changes the object's storage duration from automatic to static, meaning its lifetime spans the entire execution of the program. Toolchains commonly place zero-initialized static objects in the .bss segment and nonzero-initialized ones in .data. This introduces shared, persistent state that can break function reentrancy if concurrent accesses are not properly synchronized.
**Fix**
Recognize that static alters storage duration and persistence, not just visibility. If you only want an isolated, temporary counter per function call, drop the keyword.
```c
int counter = 0; // Fresh state on every single invocation

```
## Problem 3 — Treating const like #define
**What I wrote**
```c
const int limit = 10;
int array[limit]; // Complains under strict ISO C90

```
**What I thought**
I assumed const was just a modern, typesafe way to create a compile-time constant alias, identical to #define but cleaner.
**What actually happens**
const declares a typed object in the C language. While it may have storage and an address, it is not an integer constant expression. In strict ISO C90, you cannot use it to size static arrays. (Note: While C99 introduced Variable Length Arrays for automatic storage, it still does not convert limit into a compile-time constant expression).
**Fix**
Use #define or enum for structural macro sizes and strict compile-time constant expressions. Use const to enforce read-only access paths on variables or pointers.
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
Since the destination variable ratio is a double, I expected C to realize I wanted fractional results and evaluate the expression to 2.333333.
**What actually happens**
C evaluates expressions bottom-up based entirely on the operands, without checking where the result will be stored. Since 7 and 3 are both literal integers, C performs integer division, truncates the fractional part toward zero, gets 2, and then implicitly casts that plain 2 into 2.000000 to put it in the double.
**Fix**
Force at least one operand to be a floating-point literal so the compiler upgrades the entire calculation context to floating-point math.
```c
double ratio = 7.0 / 3;
printf("%f\n", ratio); // Correctly yields 2.333333

```
## Problem 5 — Treating % as universal mathematical modulo
**What I wrote**
```c
printf("%d\n", -7 % 3);

```
**What I thought**
I thought % operated like standard clock arithmetic (mathematical modulo), where the result is always non-negative. I expected -7 % 3 to wrap around and output 2.
**What actually happens**
In C, the % operator computes the *remainder* of integer division, not the mathematical modulo. Because integer division truncates strictly toward zero (-7 / 3 becomes -2), the remainder must preserve the negative sign of the dividend to make the algebraic identity (a / b) * b + (a % b) == a hold true. The expression evaluates to -1.
**Fix**
If you require a strictly positive mathematical modulo for wrapping indices, manually correct the negative output. **Note:** This specific correction snippet assumes a positive divisor.
```c
int rem = -7 % 3;
if (rem < 0) rem += 3; // Yields 2 (given a positive modulus)

```
## Problem 6 — Forgetting that division by zero is undefined behavior
**What I wrote**
```c
int result = a / b;

```
**What I thought**
I figured that if b happened to hit 0, C would safely return 0, set an error flag, or at worst, the compiler would catch it.
**What actually happens**
Division or remainder by zero is strictly Undefined Behavior. A typical hosted implementation may trap at runtime (often triggering a low-level OS signal like SIGFPE), but C does not guarantee a particular diagnostic, signal, or recovery path. The language relinquishes all behavioral guarantees.
**Fix**
Always guard divisions or modulus operations with a defensive logical check if the divisor originates from dynamic runtime input.
```c
int result = (b != 0) ? (a / b) : 0;

```
## Problem 7 — Confusing precedence with evaluation order
**What I wrote**
```c
int result = f() + g() * h();

```
**What I thought**
Because operator precedence dictates that multiplication (*) happens before addition (+), I assumed the CPU must physically execute g() and h() before it ever starts executing f().
**What actually happens**
Precedence only tells the compiler how to draw the structural brackets of the expression tree: f() + (g() * h()). It does *not* dictate the chronological sequence of execution for subexpressions. If the functions have side effects (like printing or altering separate state), relying on an assumed call order can produce different observable behavior. Furthermore, if unsequenced side effects modify the same scalar object, the entire expression may trigger undefined behavior.
**Fix**
Do not rely on structural operator precedence to fix execution timing. If sequence matters due to internal side effects, break the steps into separate lines.
```c
int val1 = f();
int val2 = g();
int result = val1 + (val2 * h());

```
## Problem 8 — Treating short-circuit as a performance optimization only
**What I wrote**
```c
if ((c = getchar()) != EOF && i < lim - 1)

```
**What I thought**
I flipped the layout of the K&R example because I thought checking a variable condition or fetching a char could be written in any sequence, viewing short-circuiting as just a subtle trick to skip useless work.
**What actually happens**
Because of the flipped order, getchar() is unconditionally executed first. If the buffer happens to be full (i >= lim - 1), the program will blindly pull an extra character out of the input stream anyway, and then it will find out too late that it has nowhere to safely store it. The short-circuit rule is a core control-flow guarantee.
**Fix**
Place safety guards to the left of the risky expression when the right-hand side is only valid if the guard succeeds.
```c
if (i < lim - 1 && (c = getchar()) != EOF)

```
## Problem 9 — Writing mathematical chained comparisons in C
**What I wrote**
```c
0 < x < 10

```
**What I thought**
I expected C to process this like standard mathematical notation, checking if x falls between 0 and 10.
**What actually happens**
C parses it sequentially from left to right: (0 < x) < 10. The inner expression 0 < x resolves to either integer 0 (false) or integer 1 (true). Then, the outer expression compares that boolean flag against 10. Since both 0 < 10 and 1 < 10 are logically valid and true, the complete expression always returns 1, letting completely invalid out-of-range values pass right through.
**Fix**
Explicitly split up the relational constraints using the logical && operator.
```c
0 < x && x < 10

```
## Problem 10 — Confusing = with ==
**What I wrote**
```c
if (x = 10) {
    /* Always executes */
}

```
**What I thought**
I thought the compiler would catch my typo, realize I wanted a comparison, or throw an error about using an assignment statement inside a conditional guard.
**What actually happens**
In C, an assignment expression x = 10 has a material value, which is the value being assigned (10). The conditional statement interprets that nonzero integer 10 as a blanket **true**. As a result, the loop or branch executes every single time, silently rewriting the value of x in the background and introducing subtle, frustrating bugs.
**Fix**
Use the dedicated == token for comparisons. You can also turn on compiler warnings (-Wparentheses / -Wall) to catch these assignments instantly.
```c
if (x == 10) {
    /* Executes only when x equals 10 */
}

```
## Hidden invariants
 * Automatic variables must be initialized before every read.
 * Divisors must be nonzero before / or %.
 * Conditions with side effects rely on left-to-right short-circuit evaluation.
 * Range checks must compare the same variable explicitly on both sides.
 * Parentheses may clarify grouping, but they do not always define evaluation order.
## Summary of corrections
| Problem | Correct model |
|---|---|
| Local variables | Storage exists, but value may be indeterminate |
| static | Affects storage duration and persistent state |
| const vs #define | Typed object versus textual substitution |
| Integer division | Truncates toward zero |
| % | Remainder follows C division rules |
| Division by zero | Undefined behavior |
| Precedence | Determines grouping |
| Evaluation order | Determines execution timing |
| Short-circuit | Controls whether later expressions run |
| Chained comparison | Must be written with && |
| = vs == | Assignment is not comparison |
## Final observation
The dangerous mistakes in these sections are rarely syntax errors. Most of them compile perfectly. The real problem is that the program expresses a completely different rule than the programmer intended. C will execute buggy logic with total machine efficiency if the layout matches the grammar rules. C gives you exact rules at the abstract machine level; it does not verify that your reasoning matches your intent.
