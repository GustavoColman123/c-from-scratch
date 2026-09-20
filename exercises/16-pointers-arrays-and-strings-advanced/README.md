# Pointers, Arrays, and Strings — Advanced

## Overview

This exercise continues K&R Chapter 5 by moving from basic pointer traversal into pointer-based organization and dispatch.

It covers:

* 5.6 Pointer Arrays; Pointers to Pointers
* 5.7 Multi-dimensional Arrays
* 5.8 Initialization of Pointer Arrays
* 5.9 Pointers vs Multi-dimensional Arrays
* 5.10 Command-line Arguments
* 5.11 Pointers to Functions
* 5.12 Complicated Declarations

### AI-assisted review

* GPT-6 Astra — extended post-implementation review of Day 16, used to identify candidate defects and missing contracts.
* ChatGPT — tutoring, review synthesis, documentation refinement, and verification planning.

AI review output is not treated as a normative C source. Technical claims are expected to be checked against program behavior and the language rules.

The main goal is to understand how C can organize both data and executable behavior through typed pointers.

Day 15 focused on reaching objects through addresses.

Day 16 focuses on building structures composed of pointers:

```text
pointer arrays
    ↓
pointer-to-pointer traversal
    ↓
argument vectors
    ↓
function pointers
    ↓
dispatch tables
```

## Concepts Covered

### Arrays of Pointers

* Arrays whose elements are pointer values
* Arrays of string pointers
* `const char *const`
* Variable-length strings referenced through pointer tables
* Pointer-array traversal

### Pointers to Pointers

* Multiple levels of indirection
* `*p` vs `**p`
* Traversing pointer arrays
* `char **`
* `const char *const *`

### Multi-dimensional Arrays

* Arrays whose elements are arrays
* Fixed-size rows
* Contiguous storage
* Pointer-to-array types
* Difference between `T **` and `T (*)[N]`

### Command-line Arguments

* `argc`
* `argv`
* `char *argv[]`
* parameter adjustment to `char **`
* traversing argument pointers
* traversing characters inside one argument
* `argv[argc] == NULL`

### Function Pointers

* Function-pointer declarations
* Compatible function types
* Indirect function calls
* Difference between:

```c
int *handler(void);
```

and:

```c
int (*handler)(void);
```

### Dispatch Tables

* Mapping string names to handler functions
* Structure-based command tables
* Lookup functions returning pointers
* `NULL` as lookup failure
* Dynamic command execution

## Program Structure

The exercise is implemented in one file:

```text
main.c
```

The program is divided into eight sections.

### Section 1 — Array of String Pointers

The program defines:

```c
const char *const names[] = {
    "kernel",
    "memory",
    "scheduler"
};
```

Each array element stores a pointer to a string literal.

The section demonstrates:

```c
names[1]
```

and:

```c
names[1][0]
```

showing how one expression accesses a string pointer while the other accesses a character through that pointer.

### Section 2 — Pointer to Pointer

The program uses:

```c
const char *const *cursor = names;
```

Single indirection:

```c
*cursor
```

accesses the current pointer.

Double indirection:

```c
**cursor
```

accesses the character reached through that pointer.

Advancing:

```c
cursor++;
```

moves to the next pointer element in the array.

### Section 3 — Pointer Array vs Fixed 2D Array

The program compares:

```c
const char *const pointer_commands[]
```

with:

```c
char fixed_commands[][16]
```

The pointer array stores pointer values.

The fixed 2D array directly owns character storage.

The program demonstrates that:

```c
fixed_commands[0][0] = 'H';
```

is valid because the fixed array contains writable characters.

`sizeof pointer_commands` measures only the pointer-array object itself. It does not include the storage occupied by the string literals that those pointers reference.

`sizeof fixed_commands` measures the complete `char [3][16]` array object. Because `sizeof(char)` is defined as `1`, this object occupies exactly 48 bytes.

The size of the pointer-array object depends on the implementation's pointer size, so a result such as 24 bytes must not be interpreted as the total storage cost of the pointed-to strings.

The exercise also demonstrates a pointer to one complete row:

```c
char (*row)[16] = fixed_commands;
```

`row++` advances by one `char[16]` row. This is a direct demonstration that `char **` and `char (*)[16]` are different pointer types with different traversal rules.

### Section 4 — Command-line Arguments

The program receives:

```c
int argc
```

and:

```c
char *argv[]
```

through `main`.

It prints each argument using indexed access and then traverses the same argument vector with a `char **` cursor.

This shows that command-line arguments are represented as a sequence of pointers to null-terminated strings.

### Section 5 — Search in a Pointer Table

The function:

```c
find_name(...)
```

searches a pointer table for a matching string.

If a match is found, it returns a pointer to that string.

If no match exists, it returns:

```c
NULL
```

The caller checks the result before using it.

### Section 6 — Function Pointers

The exercise declares:

```c
int (*handler)(void) = run_status;
```

This creates a pointer to a function that takes no arguments and returns `int`.

The function is then called indirectly through:

```c
handler();
```

This demonstrates that pointers can represent executable behavior as well as data relationships.

### Section 7 — Dispatch Table

The program defines:

```c
struct Command {
    const char *name;
    int (*handler)(void);
};
```

and creates:

```c
const struct Command commands[] = {
    {"help", run_help},
    {"status", run_status},
    {"version", run_version}
};
```

Each table entry connects a command name to a compatible handler function.

This makes command behavior data-driven rather than hardcoded into a long conditional chain.

### Section 8 — Mini Shell-style Dispatch

The program searches the dispatch table for command-line input.

If a matching command exists, it executes:

```c
command->handler();
```

If no match exists, it reports an unknown command and returns a non-zero status.

This combines several concepts:

```text
command-line input
    ↓
string lookup
    ↓
pointer to matching structure
    ↓
function pointer
    ↓
indirect function call
```

## Example Build

Compile:

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic main.c -o demo
```

Run with commands:

```bash
./demo help status version reboot
```

Check process exit-status propagation:

```bash
./demo help status
echo "$?"

./demo reboot
echo "$?"

./demo reboot status
echo "$?"

./demo ""
echo "$?"
```

The first execution should exit with `0`. The other three should exit with a non-zero status. The program continues executing all requested commands but preserves the first failure as the process exit status.

Remove the generated binary before committing:

```bash
rm demo
```

## Example Output

```text
=== Day 16: Pointers, Arrays, and Strings Advanced ===

=== Section 1: Array of String Pointers ===

items[0] = kernel, first char = k
items[1] = memory, first char = m
items[2] = scheduler, first char = s
names[1] = memory
names[1][0] = m

=== Section 2: Pointer to Pointer ===

*cursor = kernel
**cursor = k
after cursor++, *cursor = memory
after cursor++, **cursor = m
pointer-to-pointer walk: kernel memory scheduler

=== Section 3: Pointer Array vs Fixed 2D Array ===

pointer_commands storage = 24 bytes
fixed_commands storage   = 48 bytes
pointer_commands[0] = help
fixed_commands[0] before = help
fixed_commands[0] after  = Help

=== Section 4: Command-line Arguments ===

argc = 5
argv[0] = ./demo
argv[1] = help
argv[2] = status
argv[3] = version
argv[4] = reboot
argv pointer walk: ./demo help status version reboot

=== Section 5: Search in a Pointer Table ===

found command name: status
command name not found: reboot

=== Section 6: Function Pointers ===

calling function through handler pointer
status: all core subsystems are nominal
handler returned 0

=== Section 7: Dispatch Table ===

command table: help status version

=== Section 8: Mini Shell-style Dispatch ===

executing command: help
help: available commands are help, status, and version
return code = 0

executing command: status
status: all core subsystems are nominal
return code = 0

executing command: version
version: day16-dispatch-demo 0.1
return code = 0

unknown command: reboot
return code = 1
```

## Key Observations

* Arrays can store pointers as their elements.
* A pointer to pointer adds another level of indirection.
* `*p` and `**p` access different levels of a pointer structure.
* Pointer arrays and multi-dimensional arrays are fundamentally different.
* A pointer to pointer is not the same as a pointer to an array.
* Command-line arguments are represented through `argc` and `argv`.
* In a function parameter, `char *argv[]` is adjusted to `char **argv`.
* Function pointers refer to compatible functions.
* Parentheses are essential in function-pointer declarations.
* Lookup functions may return `NULL`.
* Dispatch tables connect string identifiers with handler functions.
* Function pointers allow runtime behavior selection through data structures.
* Concrete pointer sizes are implementation-dependent.

## Contracts and Preconditions

The helper functions in this exercise intentionally rely on explicit contracts rather than attempting to validate every possible misuse.

* String parameters passed to `string_equal`, `find_name`, and command lookup functions must point to readable null-terminated strings.
* A `(pointer, count)` traversal requires the pointer to refer to an array object with at least `count` accessible elements.
* The current `cursor = items; end = items + count;` pattern does not define `(NULL, 0)` as a supported input contract.
* Every command-table entry used for execution must contain a valid name and a non-null function pointer with a compatible type.
* `find_command` returns a borrowed pointer into the caller's table. It does not create a copy and does not extend the table's lifetime.

These contracts matter even when a program compiles cleanly and passes sanitizer runs for the tested inputs.

## Connection to Systems Programming

This exercise is directly connected to systems programming.

A command shell can use a table like:

```c
struct Command {
    const char *name;
    int (*handler)(void);
};
```

to map textual commands to handler routines.

The same table-driven design idea can appear in:

* command interpreters
* driver operation tables
* filesystem operation tables
* parser tables
* state machines
* architecture-specific handler tables
* simplified syscall or interrupt dispatch structures

Real operating-system code adds many additional concerns, including ABI rules, privilege transitions, concurrency, validation, hardware state, and architecture-specific entry paths.

This exercise does not implement those mechanisms.

It introduces the pointer and dispatch patterns that future NucleOS code can build upon.

## AI-assisted review

After the first Day 16 implementation was published, GPT-6 Astra performed an extended review of the code and documentation.

That review identified issues that had survived the initial implementation and review pass, including process exit-status propagation, an unstated `argc > 1` precondition in a documentation example, incomplete pointer/count and lifetime contracts, and an over-broad interpretation of the `sizeof` comparison.

The review also suggested demonstrating `char (*)[16]` directly. The findings were treated as review hypotheses, checked against the program and C language rules, and then incorporated into this follow-up correction.

This is the intended role of AI in this repository: not as a substitute for understanding or as a language specification, but as a reviewer that can challenge assumptions, propose counterexamples, and expose missing contracts that must still be verified by the programmer.

One design issue is intentionally deferred: `run_help` duplicates command names that already exist in the dispatch table. Removing that duplication cleanly requires reconsidering the handler interface so that help can receive command-table context. That is better addressed in a later structures or capstone exercise than by introducing a global variable here.

## Lessons Learned

Day 16 showed that pointers are useful for more than accessing memory.

They can also organize relationships between objects and represent executable behavior.

Arrays of pointers, pointer-to-pointer traversal, command-line argument vectors, function pointers, and dispatch tables all rely on the same principle:

```text
typed pointers establish explicit relationships between parts of a program
```

The main lesson is that advanced pointer code becomes manageable when every level of indirection, every array boundary, and every function type is treated as an explicit contract.

## Sources and review assistance

### Technical source

Kernighan & Ritchie, Chapter 5:

* 5.6 Pointer Arrays; Pointers to Pointers
* 5.7 Multi-dimensional Arrays
* 5.8 Initialization of Pointer Arrays
* 5.9 Pointers vs Multi-dimensional Arrays
* 5.10 Command-line Arguments
* 5.11 Pointers to Functions
* 5.12 Complicated Declarations
