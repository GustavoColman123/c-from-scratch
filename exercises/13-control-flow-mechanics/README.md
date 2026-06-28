# Control Flow Mechanics

## Overview

This exercise covers the main control-flow mechanisms from K&R Chapter 3.

It focuses on:

* 3.1 Statements and Blocks
* 3.2 If-Else
* 3.3 Else-If
* 3.4 Switch
* 3.5 Loops — While and For
* 3.6 Loops — Do-While
* 3.7 Break and Continue

The goal is not to build a complex algorithm, but to understand how C controls execution paths through statements, blocks, branches, loops, labels, and early exits.

## Concepts Covered

### Statements and Blocks

* Expression statements
* Compound statements
* Block structure
* Variables declared inside blocks

### If-Else

* Conditional execution
* `if` without `else`
* `if` with `else`
* Nested `if` statements
* Using braces to avoid ambiguity

### Else-If Chains

* Ordered decision making
* First matching condition wins
* Default behavior with final `else`

### Switch

* Selection using constant integral cases
* Multiple `case` labels sharing one action
* `default` case
* `break`
* Fall-through behavior

### Loops

* `while`
* `for`
* `do-while`
* Pre-test loops
* Post-test loops
* Centralized loop control

### Break and Continue

* Early loop termination with `break`
* Skipping the rest of an iteration with `continue`
* Difference between `break` in loops and `break` in `switch`

## Program Structure

The program is divided into eight independent sections.

### Section 1 — Statements and Blocks

```c
if (x > 0) {
    int temp = x * 2;

    printf("x is positive\n");
    printf("temporary block value = %d\n", temp);
}
```

This section demonstrates that a block groups multiple statements into one compound statement.

It also shows that a variable declared inside a block belongs to that block.

### Section 2 — If-Else and Braces

```c
if (a > 0) {
    if (b > 0) {
        printf("both are positive\n");
    }
} else {
    printf("a is not positive\n");
}
```

This section demonstrates how braces remove ambiguity in nested `if` statements.

Without braces, an `else` is associated with the nearest previous unmatched `if`.

### Section 3 — Else-If Chain

```c
if (score >= 90) {
    grade = 'A';
} else if (score >= 80) {
    grade = 'B';
} else if (score >= 70) {
    grade = 'C';
} else {
    grade = 'F';
}
```

This section demonstrates ordered decision making.

The chain stops as soon as the first true condition is found.

### Section 4 — Switch

```c
switch (c) {
case '0':
case '1':
case '2':
    digit++;
    break;
default:
    other++;
    break;
}
```

This section demonstrates selection through `case` labels.

Several labels can share the same action.

The `break` statement prevents execution from falling through into the next case.

### Section 5 — While Loop

```c
while (i < n) {
    while_sum += i;
    i++;
}
```

This section demonstrates a pre-test loop.

The condition is checked before the body executes.

If the condition is false at the beginning, the body does not run.

### Section 6 — For Loop

```c
for (i = 0; i < n; i++) {
    for_sum += i;
}
```

This section demonstrates centralized loop control.

The initialization, condition, and increment are visible in one line.

### Section 7 — Do-While Loop

```c
do {
    digits++;
    number /= 10;
} while (number > 0);
```

This section demonstrates a post-test loop.

The body executes at least once, which is useful when one pass is required even before the condition is tested.

### Section 8 — Break and Continue

```c
for (i = 0; i < 4; i++) {
    if (values[i] < 0) {
        continue;
    }

    if (values[i] == target) {
        found_index = i;
        break;
    }
}
```

This section demonstrates two control-flow escape tools.

`continue` skips the rest of the current iteration.

`break` exits the loop early when the target is found.

## Example Output

```text
=== Section 1: Statements and Blocks ===

x is positive
temporary block value = 6

=== Section 2: If-Else and Braces ===

a is not positive

=== Section 3: Else-If Chain ===

score = 85
grade = B

=== Section 4: Switch ===

character = 2
digit count = 1
other count = 0

=== Section 5: While Loop ===

sum using while = 6

=== Section 6: For Loop ===

sum using for   = 6

=== Section 7: Do-While Loop ===

digits produced for zero = 1

=== Section 8: Break and Continue ===

target = 7
found index = 2
```

## Key Observations

* A block allows several statements to behave syntactically as one compound statement.
* An `if` controls only the next statement unless braces are used.
* An `else` binds to the nearest previous unmatched `if`.
* An `else-if` chain stops after the first true condition.
* A `switch` selects a matching label but does not automatically stop after a case.
* `break` is required to prevent unwanted fall-through in `switch`.
* A `while` loop may execute zero times.
* A `do-while` loop executes at least once.
* In a `for` loop, `continue` jumps to the increment expression before the next condition test.
* `break` exits the nearest enclosing loop or switch.

## Lessons Learned

Control flow in C is grammar-driven, not indentation-driven.

Braces, statements, labels, conditions, and loop headers define the legal paths that execution may follow.

The main lesson is that control flow is not only about choosing which line runs next. It is the disciplined design of execution paths: which statements are grouped, which conditions admit entry, which branches are mutually exclusive, where fall-through is allowed, and where loops may stop or skip ahead.

## Source

Kernighan & Ritchie, Chapter 3:

* 3.1 Statements and Blocks
* 3.2 If-Else
* 3.3 Else-If
* 3.4 Switch
* 3.5 Loops — While and For
* 3.6 Loops — Do-While
* 3.7 Break and Continue
