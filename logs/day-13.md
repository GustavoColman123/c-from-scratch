# Day 13 — Control Flow Mechanics

## What I built

A lightweight bridge exercise covering the main control-flow mechanisms from K&R Chapter 3:

* 3.1 Statements and Blocks
* 3.2 If-Else
* 3.3 Else-If
* 3.4 Switch
* 3.5 Loops — While and For
* 3.6 Loops — Do-While
* 3.7 Break and Continue

The goal was not to build a complex algorithm, but to understand how C controls execution paths through statements, blocks, branches, labels, loops, and early exits.

## What changed from Day 12

Day 12 focused on expression mechanics: conversions, side effects, bitwise operators, assignment expressions, and conditional expressions.

Day 13 moved from expression-level behavior to statement-level control flow.

The central idea was:

> In C, control flow is not just about choosing which line runs next. It is the disciplined design of execution paths: which statements are grouped, which conditions admit entry, which branches are mutually exclusive, where fall-through is allowed, and where loops may stop or skip ahead.

## What I built in code

The program is divided into eight sections:

* Statements and blocks
* If-else and braces
* Else-if chains
* Switch
* While loops
* For loops
* Do-while loops
* Break and continue

The program demonstrates how control-flow structures behave in isolation, without mixing them into a large algorithm.

## What clicked

### Blocks are compound statements

A block groups multiple statements into one syntactic unit.

This matters because constructs like `if`, `while`, and `for` expect one statement.

A block allows several statements to behave as one compound statement.

### Control flow is grammar-driven

Indentation helps humans read code, but it does not define C control flow.

The dangling-else rule showed this clearly:

```c
if (a > 0)
    if (b > 0)
        printf("both positive\n");
    else
        printf("what happened?\n");
```

The `else` binds to the nearest previous unmatched `if`.

Braces are the correct way to force the intended structure.

### Else-if chains are ordered decisions

An `else-if` chain tests conditions in order.

The first true condition wins, and the rest of the chain is skipped.

This means condition order is part of the program's correctness.

### Switch uses labels, not isolated blocks

A `switch` selects a matching label.

Execution then continues from that label.

A `case` does not automatically stop execution, and it does not create an isolated scope.

This makes `break` essential for avoiding accidental fall-through.

### Loops encode repetition contracts

A `while` loop tests before the body.

A `do-while` loop executes the body before testing.

A `for` loop centralizes initialization, condition, and increment when they naturally belong together.

The form of the loop should match the structure of the problem.

### Break and continue are escape tools

`break` exits the nearest enclosing loop or switch.

`continue` skips the rest of the current iteration.

In a `for` loop, `continue` jumps to the increment expression before the next condition test.

In a `while` loop, `continue` jumps directly to the condition test.

That difference matters.

## Mistakes / confusions

* I had to stop thinking that `continue` automatically means "go to the next element."
* I had to understand that a `while` loop does not have a built-in increment section.
* I had to separate visual indentation from actual grammar.
* I had to remember that an `else` binds to the nearest unmatched `if`.
* I had to stop treating `case` labels as if they created independent blocks.
* I had to remember that `break` exits the nearest enclosing loop or switch, not necessarily the structure I emotionally intended.
* I had to be careful with macros that hide control-flow statements.

## Connection to NucleOS

Control flow is essential for systems programming.

NucleOS will eventually require code that:

* scans buffers
* parses commands
* walks arrays and tables
* checks flags and status values
* handles invalid states
* exits early when a condition is satisfied
* skips irrelevant data
* dispatches behavior based on state

Examples of future patterns:

```c
while ((c = getchar()) != EOF) {
    /* process input */
}
```

```c
for (i = 0; i < limit; i++) {
    if (table[i] == target)
        break;
}
```

```c
switch (state) {
case READY:
    /* run task */
    break;
case BLOCKED:
    /* skip task */
    break;
default:
    /* invalid state */
    break;
}
```

These are not just beginner control structures.

They are the skeleton of parsers, schedulers, scanners, state machines, and low-level execution paths.

## Final reflection

Day 13 was lightweight, but important.

The main lesson is that C control flow must be designed explicitly.

Statements, blocks, conditions, labels, loops, `break`, and `continue` form the legal paths that execution can follow.

This matters because arrays, strings, pointers, buffers, and operating-system code all depend on correct execution paths.

Before manipulating memory directly, I need to know exactly how execution reaches that memory, when it skips it, and when it exits.

## Time

Light study session — focused reading, question review, implementation, problems, notes, and documentation.
