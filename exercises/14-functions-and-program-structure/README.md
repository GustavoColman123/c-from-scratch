# Functions and Program Structure

## Overview

This exercise covers the main program-structure mechanisms from K&R Chapter 4.

It focuses on:

* 4.1 Basics of Functions
* 4.2 Functions Returning Non-integers
* 4.3 External Variables
* 4.4 Scope Rules
* 4.5 Header Files
* 4.6 Static Variables
* 4.7 Register Variables
* 4.8 Block Structure
* 4.9 Initialization
* 4.10 Recursion
* 4.11 The C Preprocessor

The goal is not to build a large algorithm.

The goal is to understand how C programs are divided into functions, declarations, definitions, headers, source files, translation units, and private module state.

## Concepts Covered

### Functions

* Function definitions
* Function declarations
* Prototypes
* Caller and callee
* Parameters passed by value
* Return values
* Missing return paths in non-void functions

### Program Structure

* Multi-file C programs
* Header files
* Source files
* Translation units
* Linking multiple `.c` files
* Public interface vs private implementation

### Linkage and State

* External variables
* `extern`
* File-scope `static`
* Static helper functions
* Private module state
* Internal linkage

### Preprocessor Basics

* `#include`
* `#define`
* Header guards
* Conditional compilation
* Macro risks

## Program Structure

The exercise is divided into three files:

```text
main.c
flags.c
flags.h
```

### `flags.h`

The header contains the public interface of the flag module.

```c
#ifndef FLAGS_H
#define FLAGS_H

#define FLAGS_MAX_BITS 8u

#define FLAGS_READY 0u
#define FLAGS_ERROR 1u
#define FLAGS_DEBUG 2u

void flags_reset(void);

int flags_set(unsigned bit);
int flags_clear(unsigned bit);
int flags_toggle(unsigned bit);
int flags_is_set(unsigned bit);

unsigned flags_snapshot(void);

#endif
```

This file exposes constants and function prototypes.

It does not expose the internal variable that stores the flags.

### `flags.c`

The implementation file owns the module state.

```c
#include "flags.h"

static unsigned flags = 0;
```

The variable `flags` has internal linkage because it is marked `static`.

Only `flags.c` can name it directly.

The file also contains private helper functions:

```c
static int valid_bit(unsigned bit)
{
    return bit < FLAGS_MAX_BITS;
}
```

```c
static unsigned mask_for(unsigned bit)
{
    return 1u << bit;
}
```

These helpers are implementation details.

They are not part of the public interface.

### `main.c`

The main program uses the module through the header.

```c
#include "flags.h"
```

It calls public functions such as:

```c
flags_set(FLAGS_READY);
flags_toggle(FLAGS_DEBUG);
flags_clear(FLAGS_READY);
```

It never accesses the private `flags` object directly.

## Example Build

Compile both source files:

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic main.c flags.c -o demo
```

Run the program:

```bash
./demo
```

Remove the generated binary before committing:

```bash
rm demo
```

## Example Output

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

## Key Observations

* A function declaration describes an interface.
* A function definition provides an implementation.
* Parameters are passed by value.
* `return` sends control and a value back to the caller.
* A header exposes the public contract of a module.
* A `.c` file contains implementation details.
* `#include` performs textual inclusion before compilation.
* Each `.c` file becomes a separate translation unit.
* The linker connects compiled object files.
* File-scope `static` gives internal linkage.
* Private helper functions should usually be marked `static`.
* `extern` cannot access a symbol with internal linkage.
* Macros are text substitutions, not functions.

## Connection to Systems Programming

This exercise is a small model of systems-level modularity.

Operating-system code cannot be one giant source file.

A kernel needs modules that expose safe operations while hiding implementation details.

The flag module demonstrates that pattern:

```c
static unsigned flags = 0;
```

is private state.

```c
int flags_set(unsigned bit);
```

is public behavior.

This pattern can later be reused in NucleOS for schedulers, memory managers, command parsers, drivers, and kernel state modules.

## Lessons Learned

Day 14 showed that C program structure is built from explicit boundaries.

Functions define caller/callee boundaries.

Headers define public interfaces.

Source files define implementation units.

`static` protects internal state.

The preprocessor transforms source before compilation.

The main lesson is that writing C is not only about individual statements.

It is also about designing the boundaries between pieces of a program.

## Source

Kernighan & Ritchie, Chapter 4:

* 4.1 Basics of Functions
* 4.2 Functions Returning Non-integers
* 4.3 External Variables
* 4.4 Scope Rules
* 4.5 Header Files
* 4.6 Static Variables
* 4.7 Register Variables
* 4.8 Block Structure
* 4.9 Initialization
* 4.10 Recursion
* 4.11 The C Preprocessor
