# Notes — Control Flow Mechanics

## Source

K&R Chapter 3, sections 3.1–3.7.

This is a lightweight bridge note about how C controls execution.

The focus is not on complex algorithms, but on the basic structures that determine which statements execute, when loops repeat, and where execution may exit early.

## 1. Statements and blocks

A statement is a unit of execution.

An expression followed by a semicolon becomes an expression statement:

```c
x = 0;
i++;
printf("hello\n");
```

The semicolon terminates the expression statement.

If the expression produces a value and that value is not used, the value is discarded.

A block, also called a compound statement, groups several statements into one syntactic unit:

```c
{
    statement1;
    statement2;
}
```

This matters because C often expects one statement after constructs like `if`, `while`, or `for`.

A block can be used wherever a single statement is expected.

### Key insight

**Blocks make multiple statements behave syntactically as one statement.**

## 2. Block scope

A variable declared inside a block has block scope.

Example:

```c
if (x > 0) {
    int temp = x * 2;
    printf("%d\n", temp);
}

printf("%d\n", temp); /* invalid */
```

The name `temp` is visible only from its declaration to the end of the block.

For an ordinary automatic variable, the object also stops existing when execution leaves the block.

### Important distinction

Scope and lifetime are related, but they are not the same thing.

* Scope controls where a name can be used.
* Lifetime controls how long the object exists.

## 3. If-else

The basic form is:

```c
if (expression)
    statement
else
    statement
```

The `else` part is optional.

In C, zero is false and any nonzero value is true.

Example:

```c
if (x)
    printf("true\n");
```

This prints when `x` is any nonzero value.

### Dangling else

Nested `if` statements can be ambiguous to a human reader:

```c
if (a > 0)
    if (b > 0)
        printf("both positive\n");
    else
        printf("what happened?\n");
```

The rule is:

```text
An else is associated with the nearest previous unmatched if.
```

Therefore, the `else` belongs to:

```c
if (b > 0)
```

not to:

```c
if (a > 0)
```

Braces remove the ambiguity:

```c
if (a > 0) {
    if (b > 0) {
        printf("both positive\n");
    }
} else {
    printf("a is not positive\n");
}
```

### Key insight

**Control flow in C is grammar-driven, not indentation-driven.**

## 4. Else-if chains

An `else-if` chain expresses ordered multi-way decision making.

Example:

```c
if (score >= 90)
    grade = 'A';
else if (score >= 80)
    grade = 'B';
else if (score >= 70)
    grade = 'C';
else
    grade = 'F';
```

The conditions are tested in order.

The first true condition wins.

After one branch executes, the rest of the chain is skipped.

### Order matters

This is wrong:

```c
if (score >= 70)
    grade = 'C';
else if (score >= 80)
    grade = 'B';
else if (score >= 90)
    grade = 'A';
```

If `score` is `95`, the first condition is already true, so the result is `C`.

The more restrictive cases must come before the more general ones.

### Key insight

**An else-if chain is an ordered set of mutually exclusive decisions.**

## 5. Switch

A `switch` selects execution based on one controlling expression.

Example:

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

The expression is evaluated once.

If a matching `case` label exists, execution continues from that label.

Several labels can share one action:

```c
case '0':
case '1':
case '2':
    digit++;
    break;
```

This works because `case` labels are labels, not isolated blocks.

### Fall-through

A `case` does not automatically stop execution.

Without `break`, execution continues into the following case.

```c
case '2':
    digit++;
default:
    other++;
```

If `c` is `'2'`, both `digit++` and `other++` execute.

This is fall-through.

Sometimes fall-through is intentional, but it should be used sparingly and documented clearly.

### Key insight

**A switch selects an entry point, but break controls the exit.**

## 6. While loops

A `while` loop tests the condition before executing the body:

```c
while (i < n) {
    sum += i;
    i++;
}
```

If the condition is false at the beginning, the body runs zero times.

### Key insight

**A while loop is a pre-test loop.**

## 7. For loops

A `for` loop centralizes loop control:

```c
for (i = 0; i < n; i++) {
    sum += i;
}
```

The three parts are:

```text
initialization; condition; increment
```

The order is:

```text
1. initialization
2. condition test
3. loop body
4. increment expression
5. condition test again
```

A simple `for` loop can often be rewritten as a `while` loop:

```c
i = 0;
while (i < n) {
    sum += i;
    i++;
}
```

However, care is needed when `continue` is involved.

In a `for` loop, `continue` jumps to the increment expression before the next condition test.

### Key insight

**A for loop is best when initialization, test, and update belong together.**

## 8. Do-while loops

A `do-while` loop tests the condition after executing the body:

```c
do {
    statement;
} while (expression);
```

The body always executes at least once.

This is useful when one pass is required before the condition can decide whether another pass is needed.

Example:

```c
do {
    digits++;
    n /= 10;
} while (n > 0);
```

This pattern is useful for converting the number `0` into the character `'0'`.

A regular `while (n > 0)` loop would skip the body when `n` is zero.

### Risk

A `do-while` loop is safe only when the first execution of the body is valid.

If the body assumes a condition that has not been checked yet, the program may access invalid data before the test occurs.

### Key insight

**A do-while loop is a post-test loop.**

## 9. Break and continue

`break` exits the nearest enclosing loop or `switch`.

Example:

```c
while (i < n) {
    if (array[i] == target)
        break;

    i++;
}
```

When `target` is found, the loop stops immediately.

`continue` skips the rest of the current iteration.

Example:

```c
for (i = 0; i < n; i++) {
    if (array[i] < 0)
        continue;

    sum += array[i];
}
```

Negative values are skipped.

The loop itself continues with the next iteration.

### Important distinction

In a `switch`, `break` prevents fall-through.

In a loop, `break` terminates repetition.

If a `switch` is inside a loop, a `break` inside the `switch` exits only the `switch`, not the loop.

### Key insight

**Break exits a structure; continue skips to the next iteration.**

## 10. Connection to NucleOS

Control flow is essential for systems programming.

NucleOS will require control structures for:

* parsing commands
* scanning buffers
* walking arrays
* searching tables
* checking status flags
* handling error paths
* exiting early when a condition is satisfied

Examples:

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

These patterns are not just beginner syntax.

They are the basic structure of parsers, schedulers, scanners, state machines, and low-level control paths.

## Practical takeaways

1. A block groups multiple statements into one compound statement.
2. Variables declared inside a block have block scope.
3. In C, zero is false and nonzero is true.
4. An `else` binds to the nearest previous unmatched `if`.
5. An `else-if` chain stops after the first true condition.
6. The order of conditions matters.
7. `switch` uses labels, and labels do not stop execution.
8. `break` prevents unwanted fall-through in `switch`.
9. `while` may execute zero times.
10. `do-while` executes at least once.
11. `continue` behaves differently in `for` and `while`.
12. `break` exits the nearest enclosing loop or switch.

## Final observation

Control flow in C is not just about choosing which line runs next.

It is the disciplined design of execution paths: which statements are grouped, which conditions admit entry, which branches are mutually exclusive, where fall-through is allowed, and where loops may stop or skip ahead.

This matters because pointers, arrays, strings, parsers, buffers, and operating-system code all depend on well-designed execution paths.
