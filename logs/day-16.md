# Day 16 — Pointers, Arrays, and Strings (Advanced)

## What I built

A single-file C program demonstrating advanced pointer mechanics, including arrays of string pointers, pointers to pointers, command-line arguments, table searches, function pointers, and dispatch tables.

The program covers:

* arrays of string pointers
* pointer-to-pointer traversal
* fixed multi-dimensional arrays
* pointer arrays vs fixed 2D arrays
* command-line argument processing with `argc` and `argv`
* string-table lookup patterns
* function-pointer declarations and invocation
* dispatch tables mapping strings to compatible handler functions
* mini shell-style command execution

## What changed from Day 15

Day 15 covered foundational pointer mechanics:

* address-of and indirection
* pointer arithmetic
* array traversal
* output pointers
* strings and null termination
* pointer-based string processing

Day 16 moved from traversing individual objects to organizing collections of addresses and executable behavior.

The central shift was:

```text
pointer traversal -> pointer-based organization and dispatch
```

Day 15 was about reaching objects through addresses.

Day 16 was about building structured pointer tables, argument vectors, and dispatch mechanisms.

## What I built in code

### Section 1 — Array of String Pointers

The program defines:

```c
const char *const names[] = {
    "kernel",
    "memory",
    "scheduler"
};
```

This is an array whose elements are pointers to constant characters.

The pointer values themselves are also constant through this declaration.

The program prints individual strings and accesses characters through two levels of indexing:

```c
names[1]
```

refers to:

```text
memory
```

while:

```c
names[1][0]
```

produces:

```text
m
```

### Section 2 — Pointer to Pointer

The program uses:

```c
const char *const *cursor = names;
```

to traverse the pointer array.

Single indirection:

```c
*cursor
```

accesses the current string pointer.

Double indirection:

```c
**cursor
```

accesses the current character reached through that string pointer.

Advancing:

```c
cursor++;
```

moves to the next pointer element in the array.

### Section 3 — Pointer Array vs Fixed 2D Array

The exercise compares:

```c
const char *const pointer_commands[]
```

with:

```c
char fixed_commands[][16]
```

The pointer-array version stores pointer values.

In this exercise, those pointers refer to string literals, which must not be modified.

The fixed 2D array directly owns writable character storage.

This makes:

```c
fixed_commands[0][0] = 'H';
```

valid.

On the machine used for this exercise, the output was:

```text
pointer_commands storage = 24 bytes
fixed_commands storage   = 48 bytes
```

The `24`-byte result reflects the pointer size on this particular implementation.

C does not guarantee that pointers are always 8 bytes.

### Section 4 — Command-line Arguments

The program inspects:

```c
int argc
```

and:

```c
char *argv[]
```

from `main`.

It traverses arguments with normal indexing and with a `char **` cursor.

The execution:

```bash
./demo help status version reboot
```

produced five argument strings, including the program invocation name.

### Section 5 — Search in a Pointer Table

`find_name` scans an array of string pointers.

If a match is found, it returns a pointer to the matching string.

If no match exists, it returns:

```c
NULL
```

The caller checks the result before using it.

### Section 6 — Function Pointers

The program declares:

```c
int (*handler)(void) = run_status;
```

This creates a pointer to a function compatible with:

```c
int function_name(void);
```

The function is then called indirectly:

```c
handler();
```

This demonstrates that pointers can represent callable behavior, not only data objects.

### Section 7 — Dispatch Table

The program defines:

```c
struct Command {
    const char *name;
    int (*handler)(void);
};
```

and builds:

```c
const struct Command commands[] = {
    {"help", run_help},
    {"status", run_status},
    {"version", run_version}
};
```

Each table entry connects a command name to a compatible function.

Behavior selection is therefore represented as data rather than as a long conditional chain.

### Section 8 — Mini Shell-style Dispatch

`execute_command` searches the dispatch table by name.

If a command exists, the program invokes:

```c
command->handler();
```

If the lookup fails, the function reports the unknown command and returns a non-zero status code.

This combines:

```text
command-line input
    ↓
string lookup
    ↓
pointer to command structure
    ↓
function pointer
    ↓
indirect function call
```

## Example output

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

## What clicked

### Pointer arrays organize references

An array does not need to contain the objects themselves.

It can contain pointers to separate objects.

This makes variable-length strings and shared objects easier to organize.

### Pointers to pointers represent another traversal level

A `char *` can traverse characters.

A `char **` can traverse pointers to strings.

The pointer level must match the structure being traversed.

### Multi-dimensional arrays are not pointer-to-pointer objects

A declaration such as:

```c
char table[3][16];
```

creates an array whose elements are arrays.

It does not create an array of independent pointers.

Its pointer type after array-to-pointer conversion is fundamentally different from `char **`.

### Storage layout matters

Fixed 2D arrays own contiguous character storage.

Pointer arrays store pointer values whose targets may exist elsewhere.

Neither representation is universally better.

The correct choice depends on ownership, mutability, dimensions, and lifetime requirements.

### `argc` and `argv` form an argument vector

Command-line arguments arrive as separate strings reached through an array-like sequence of character pointers.

This is a direct practical use of pointers to pointers.

### Function pointers represent behavior

A function pointer can select which compatible routine will execute.

The pointer's type describes the function-call contract.

### Dispatch tables turn behavior selection into data

Combining names and function pointers inside structures creates a data-driven command interface.

The main dispatch logic no longer needs a separate branch for every command.

## Mistakes / confusions

* I had to remember that string literals must not be modified.
* I had to distinguish pointer arrays from multi-dimensional arrays.
* I had to distinguish `char *` from `char **` when traversing argument vectors.
* I had to understand that pointer-to-pointer types are not interchangeable with pointers to arrays.
* I had to use parentheses correctly in function-pointer declarations such as `int (*handler)(void)`.
* I had to remember that array parameters do not preserve the original array extent.
* I had to remember that table lookups can return `NULL`.
* I had to check lookup results before accessing structures or invoking handlers.
* I had to separate implementation-specific pointer sizes from rules guaranteed by C.

## Connection to NucleOS

This exercise connects directly to future NucleOS architecture.

A small kernel shell could use a command table such as:

```c
struct Command {
    const char *name;
    int (*handler)(void);
};
```

User input could be parsed into a command name, searched in a static table, and dispatched to a compatible handler function.

The same underlying table-driven idea can appear in future:

* command interfaces
* driver operation tables
* parser tables
* state machines
* filesystem operation tables
* architecture-specific handler tables
* simplified syscall or interrupt dispatch structures

A real kernel adds much more complexity, including privilege transitions, ABI rules, concurrency, validation, architecture-specific entry paths, and hardware state.

Day 16 does not implement those mechanisms.

It establishes the pointer and dispatch concepts that such systems can build upon.

## Final reflection

Day 16 bridged low-level pointer handling with systems organization.

Day 15 showed how pointers reach and traverse objects.

Day 16 showed how pointers can organize relationships between objects and even represent executable behavior.

Arrays of pointers, multiple levels of indirection, command-line arguments, function pointers, and dispatch tables all rely on the same fundamental idea: typed pointers establish explicit relationships between pieces of a program.

This moves the project from basic pointer manipulation toward modular, table-driven systems architecture.

## Time

4 hours — reading, conceptual review, implementation, output testing, problems, notes, and documentation.
