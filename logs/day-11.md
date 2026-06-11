# Day 11 — Declarations, Arithmetic Operators, and Logical Expressions

## What I built

A clean, deliberate demonstration program targeting K&R Chapter 2.4–2.6. The program isolates and exercises specific compiler behaviors rather than just printing calculations:

* **Storage boundaries:** Contrasting automatic stack allocation with zero-initialized static memory.
* **Constant evaluation:** Probing why a typed `const int` object behaves differently from a `#define` text macro during compilation.
* **Arithmetic truncation:** Demonstrating integer division truncation toward zero and validating the algebraic remainder identity `(a / b) * b + (a % b) == a` across both positive and negative values.
* **Control flow in disguise:** Testing relational expressions by observing literal `0` and `1` results and validating short-circuit boundaries using operands with side effects.
* **Logical edge cases:** Isolating the chained-comparison failure mode `0 < x < 10` without polluting the terminal build with warnings.

## What changed from Day 10

Day 10 focused strictly on data representation, types, sign extension, and literal sizes in memory. Day 11 applies those static concepts to dynamic expressions.

The primary question has shifted from a passive:

> What type and bit layout does this object have?

To an active, execution-driven:

> How does the abstract machine parse this expression, in what sequence does it execute subexpressions, and what optimization assumptions is the compiler allowed to make?

## What I had to define before writing code

* **Safety isolation:** I had to ensure that the uninitialized automatic-variable example was isolated or commented out, preventing accidental UB during the demonstration and avoiding optimizer assumptions based on undefined behavior.
* **Test matrix for truncation:** I had to choose a fixed set of matching positive and negative operands, such as `7`, `-7`, and `3`, to make C's truncation-toward-zero rule and the relationship between `/` and `%` visually obvious in the terminal output.
* **Warning management:** I had to structure the `0 < x < 10` trap carefully. Instead of leaving a raw expression that triggers a compiler warning, I encapsulated the logical evaluation inside a dedicated test function to show its output cleanly.
* **Build standard:** I enforced strict compiler flags: `-Wall -Wextra -Wpedantic -std=c17`.

## What clicked

### Storage allocation is not initialization

An object can exist in the abstract machine without containing a valid, usable value. When I declare `int x;` inside a function, the stack-frame storage is allocated, but its value is indeterminate. Reading it is not merely reading old data; it invokes undefined behavior.

### Short-circuit is true control flow

In the expression `i < lim - 1 && (c = getchar()) != EOF`, the left side is not merely checked first as a performance shortcut. It acts as an operational guard rail. If the index check fails, the right side never executes. Short-circuiting is not a minor optimization; it is a control-flow mechanism that prevents input consumption before capacity has been validated.

### Valid syntax can encode completely invalid reasoning

Writing `0 < x < 10` is valid C grammar. The compiler accepts it because the syntax rules are satisfied. However, it translates to `(0 < x) < 10`, which is always true because the inner expression produces either `0` or `1`. This taught me that the compiler is a gatekeeper for language structure, not a reviewer of logical intent.

## Key difference in thinking

* **Before:** I viewed operators as simple arithmetic and logical symbols with static, memorized meanings pulled from mathematics.
* **Now:** I see operators as nodes that build an expression tree. Each node introduces grouping constraints, possible conversion rules, evaluation-order concerns, and conditional execution paths.

## Mistakes / confusions

* **Static scope vs. storage:** I initially treated `static` mainly as a visibility modifier. I had to recognize that it changes storage duration and introduces persistent state.
* **Precedence vs. execution sequence:** I caught myself assuming that because multiplication has higher precedence than addition, the multiplication operands must execute first. I had to separate grouping from execution order.
* **Modulo vs. remainder:** Coming from languages where `%` is often used as a nonnegative modulo operator, I was confused by negative outputs until I mapped out C's truncation rules.

## Connection to systems programming

These basic mechanics are where real systems bugs begin. C prioritizes direct control and minimal runtime overhead, so syntactically valid but logically incorrect code can compile efficiently.

An uninitialized read may expose stale stack data or be optimized unpredictably. Reordering a short-circuit guard may consume input before confirming capacity. Dividing by a runtime zero value invokes undefined behavior and may trap on typical systems. At the systems level, there is often no managed runtime to catch these mistakes automatically.

## What still feels unresolved

* **Exploitation of UB:** How aggressively do modern compilers such as GCC and Clang optimize around uninitialized reads?
* **Assembly generation:** How drastically does generated assembly change when expression trees are rearranged by the compiler?
* **Branchless short-circuits:** When does a compiler use conditional jumps for `&&` and `||`, and when can it use branchless instructions safely?
* **Concurrency risks:** At what point do local static variables in bare-metal firmware stop being convenient state and become a reentrancy or synchronization problem?

## Final reflection

The true challenge of these sections was not learning operator symbols or declaration syntax. It was realizing how easily a developer can write clean, warning-free C code that looks mathematically reasonable while expressing a completely different rule to the abstract machine.

C provides precise mechanics. It does not assume responsibility for the programmer's logic.

## Time

**Total time invested:** 3.5 hours — reading, testing isolated compiler warnings, and validating behavior under different flags.
