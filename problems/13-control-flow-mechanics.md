# Problems — Day 13 (Control Flow Mechanics)

## Problem 1 — Using `continue` in a `while` loop before updating the index

**What I wrote**

```c
int i = 0;

while (i < length) {
    if (text[i] == '\r' || text[i] == '\t') {
        continue; /* skip control characters */
    }

    process(text[i]);
    i++;
}
```

**What I thought**

I thought `continue` meant "skip this character and move to the next one."

My mental model was that the loop would automatically continue with the next element of the string.

**What actually happens**

In a `while` loop, `continue` does not update the loop variable.

It skips the rest of the current iteration and jumps directly back to the loop condition:

```c
while (i < length)
```

That means this statement is skipped:

```c
i++;
```

If `text[i]` is `'\r'` or `'\t'`, the program executes `continue`, returns to the condition, and tests the same value of `i` again.

Since `i` never changes, the same character is tested forever.

The result is an infinite loop.

**Fix**

Update the index before using `continue`:

```c
int i = 0;

while (i < length) {
    if (text[i] == '\r' || text[i] == '\t') {
        i++;
        continue;
    }

    process(text[i]);
    i++;
}
```

Or use a `for` loop when initialization, condition, and increment naturally belong together:

```c
for (int i = 0; i < length; i++) {
    if (text[i] == '\r' || text[i] == '\t') {
        continue;
    }

    process(text[i]);
}
```

In a `for` loop, `continue` jumps to the increment expression before the next condition test.

**Lesson**

`continue` means "skip the rest of this iteration."

It does not mean "advance the loop variable" unless the loop structure already guarantees that update.

## Problem 2 — Declaring a variable immediately after a `case` label

**What I wrote**

```c
switch (command) {
case CMD_START:
    int status = start_system();

    if (status == OK) {
        run_process();
    }

    break;

case CMD_STOP:
    stop_system();
    break;
}
```

**What I thought**

I thought each `case` behaved like an independent block.

My mental model was close to this:

```c
if (command == CMD_START) {
    int status = start_system();
    ...
} else if (command == CMD_STOP) {
    stop_system();
}
```

So I expected `status` to belong only to the `CMD_START` case.

**What actually happens**

A `switch` is not a chain of isolated blocks.

The `case` labels are labels inside the body of the `switch`.

In C, a label must label a statement. A declaration is not an expression statement.

So this is not valid in C17:

```c
case CMD_START:
    int status = start_system();
```

The declaration appears immediately after the label.

There is also a scope issue: without braces, declarations inside one case may belong to the surrounding `switch` block, not to an isolated case-specific block.

The compiler is rejecting code whose structure does not create a proper block for the local variable.

**Fix**

Use braces to create an explicit block for the case:

```c
switch (command) {
case CMD_START: {
    int status = start_system();

    if (status == OK) {
        run_process();
    }

    break;
}

case CMD_STOP:
    stop_system();
    break;

default:
    break;
}
```

Now `status` belongs only to the block inside `case CMD_START`.

Its scope is limited to that block.

**Lesson**

A `case` label is not a block.

If a case needs local declarations, create an explicit block with braces.

## Problem 3 — Hiding `break` inside a macro used inside a `switch`

**What I wrote**

```c
#define VALIDATE_RECORD(r) if ((r).invalid) break

for (int i = 0; i < total; i++) {
    switch (port[i].type) {
    case ETHERNET:
        VALIDATE_RECORD(port[i]);
        transmit(port[i]);
        break;
    }
}
```

**What I thought**

I wanted the macro to stop processing the current loop when the record was invalid.

My mental model was:

```c
if (port[i].invalid)
    break; /* exit the for loop */
```

So I expected the `break` inside the macro to exit the `for`.

**What actually happens**

The preprocessor performs textual substitution before the compiler analyzes control flow.

After macro expansion, the code effectively becomes:

```c
for (int i = 0; i < total; i++) {
    switch (port[i].type) {
    case ETHERNET:
        if (port[i].invalid)
            break;

        transmit(port[i]);
        break;
    }
}
```

The `break` is inside a `switch`.

In C, `break` exits the nearest enclosing `switch`, `while`, `for`, or `do-while`.

The nearest valid target is the `switch`, not the outer `for`.

Therefore, when `port[i].invalid` is true, the macro exits only the `switch`.

The `for` loop continues with the next iteration.

**Fix**

Do not hide control-flow exits like `break` inside macros unless the target structure is completely obvious.

One clearer approach is to validate before the `switch`:

```c
for (int i = 0; i < total; i++) {
    if (port[i].invalid) {
        break;
    }

    switch (port[i].type) {
    case ETHERNET:
        transmit(port[i]);
        break;

    default:
        break;
    }
}
```

If the goal is to skip only the current record, use `continue` at the loop level:

```c
for (int i = 0; i < total; i++) {
    if (port[i].invalid) {
        continue;
    }

    switch (port[i].type) {
    case ETHERNET:
        transmit(port[i]);
        break;

    default:
        break;
    }
}
```

Another option is to avoid the macro and use a function that returns a status value:

```c
int valid_record(struct port p)
{
    return !p.invalid;
}
```

Then the caller decides the control flow explicitly.

**Lesson**

A macro does not create a new control-flow rule.

After expansion, `break` still follows normal C semantics.

It exits the nearest enclosing `switch` or loop, which may not be the structure the programmer had in mind.

## Hidden invariants

* `continue` skips the rest of the current iteration.
* In a `while` loop, `continue` jumps directly to the condition test.
* In a `for` loop, `continue` jumps to the increment expression before the next condition test.
* A `case` label is not a block.
* A declaration immediately after a `case` label is invalid in C17 unless a proper statement/block structure is provided.
* Braces inside a `case` create a real block and limit scope.
* `break` exits the nearest enclosing `switch` or iteration statement.
* A `break` inside a `switch` nested in a loop exits the `switch`, not the loop.
* Macros are expanded before control-flow analysis.
* Hidden control flow inside macros can make code misleading.

## Summary of corrections

| Problem                        | Correct model                                                         |
| ------------------------------ | --------------------------------------------------------------------- |
| `continue` in `while`          | It skips to the condition; it does not automatically update the index |
| Local declaration after `case` | A `case` label is not a block; use braces for case-local declarations |
| `break` inside macro           | After expansion, `break` exits the nearest enclosing switch or loop   |

## Final observation

The dangerous part of control flow in C is that the code can look visually obvious while the grammar says something different.

Indentation does not bind `else`.

A `case` does not create a block.

A macro does not preserve intention.

A `break` does not exit the structure the programmer emotionally meant; it exits the nearest structure allowed by the language.

Day 13 showed that C control flow must be designed as explicit execution paths, not as visual formatting.
