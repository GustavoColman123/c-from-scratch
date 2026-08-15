# Day 14 — Functions and Program Structure

## What I built

A small multi-file C program that implements a private flag module.

The program is divided into:

```text
main.c
flags.c
flags.h
```

The goal was to practice functions, declarations, definitions, header files, file-scope `static`, private helper functions, return values, and basic preprocessor usage.

## What changed from Day 13

Day 13 focused on control flow inside one function.

Day 14 moved into program structure across functions and files.

The central shift was:

> control flow inside a function -> architecture across functions and translation units

Day 13 was about designing execution paths.

Day 14 was about designing boundaries.

## What I built in code

### `flags.h`

The header exposes the public interface of the flag module.

It contains:

* a header guard
* symbolic constants
* public function prototypes such as `flags_set`, `flags_clear`, `flags_toggle`, `flags_is_set`, and `flags_snapshot`

This file tells other translation units what they are allowed to use.

It does not expose the private state.

### `flags.c`

The implementation file owns the actual module state.

It contains:

```c
static unsigned flags = 0;
```

This object is private to `flags.c`.

The file also contains private helper functions such as:

```c
static int valid_bit(unsigned bit)
```

and:

```c
static unsigned mask_for(unsigned bit)
```

These helpers are implementation details.

The public functions declared in `flags.h` are defined in `flags.c`.

### `main.c`

The main program includes the header:

```c
#include "flags.h"
```

Then it uses the flag module only through the public functions.

It does not touch `flags` directly.

This is the main architectural point of the exercise: the caller uses the interface, not the implementation.

## Example output

```text
=== Day 14: Functions and Program Structure ===

Initial state
raw flags = 0
READY = off
ERROR = off
DEBUG = off

After setting READY and DEBUG
raw flags = 5
READY = on
ERROR = off
DEBUG = on

After toggling DEBUG
raw flags = 1
READY = on
ERROR = off
DEBUG = off

After clearing READY
raw flags = 0
READY = off
ERROR = off
DEBUG = off

invalid bit rejected

After reset
raw flags = 0
READY = off
ERROR = off
DEBUG = off
```

## What clicked

### Functions are boundaries

Functions establish the boundary between caller and callee.

The caller passes argument values.

The callee receives those values through parameters, works with its own local objects, and may return a result.

The function call is the crossing point between those two sides.

### Headers are public contracts

`flags.h` acts as the public contract of the module.

It exposes what another file is allowed to call.

It does not expose how the module stores or manipulates its state internally.

### Source files are implementation units

`flags.c` owns the real implementation.

It contains the private state, private helpers, and public function definitions.

After preprocessing, it becomes its own translation unit.

### `static` creates privacy at file scope

At file scope, `static` gives internal linkage.

That means:

```c
static unsigned flags = 0;
```

can be named only inside `flags.c`.

`main.c` cannot access it directly.

This is one of the most important tools for building modules in C.

### Return values can report success or failure

Functions like:

```c
int flags_set(unsigned bit);
```

return a status value.

If the bit is valid, the function changes the internal state and returns success.

If the bit is invalid, the function rejects the operation.

This prevents invalid input from corrupting private state.

### The preprocessor works before the compiler

Directives like:

```c
#include "flags.h"
```

perform textual inclusion before compilation.

`#define` creates symbolic substitutions.

Header guards prevent repeated header processing inside one translation unit.

The preprocessor transforms the source before the compiler checks C semantics.

## Mistakes / confusions

* I had to separate function declaration from function definition.
* I had to understand that headers expose contracts, not implementations.
* I had to remember to compile both `main.c` and `flags.c`.
* I had to understand that `static` at file scope hides a symbol from other files.
* I had to stop treating `#include` like a Python import.
* I had to remember that macros are textual substitution.
* I had to understand that `extern` cannot access a file-scope `static` object.
* I had to think about C programs as translation units connected by the linker.

## Connection to NucleOS

This exercise is indirectly connected to NucleOS because operating-system code is modular.

A kernel cannot be one giant `main.c`.

It needs modules that expose safe public operations while hiding internal state.

The flag module is a small version of that idea.

```c
static unsigned flags = 0;
```

is private state.

```c
int flags_set(unsigned bit);
```

is public behavior.

Future systems code will use similar boundaries for schedulers, memory managers, device drivers, command parsers, and kernel state.

A scheduler might expose functions for adding or selecting tasks while hiding its internal queues.

A memory manager might expose allocation functions while hiding its free lists.

A driver might expose read/write operations while hiding hardware-specific details.

Day 14 is a small foundation for that style of C architecture.

## Final reflection

Day 14 showed that C program structure is built from explicit boundaries.

Functions define caller/callee boundaries.

Headers define public interfaces.

Source files define implementation units.

`static` protects internal state.

`extern` refers to external definitions but cannot bypass internal linkage.

The preprocessor transforms source text before compilation.

This is the first step from writing C programs to designing C modules.

## Time

3.5 hours — reading, question review, implementation, output testing, problems, notes, and documentation.
