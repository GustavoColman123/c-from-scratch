# Problems — Day 14 (Functions and Program Structure)

## Problem 1 — Calling a function without a visible prototype

**What I wrote**

```c
/* main.c */
int main(void)
{
    double result = calculate_average(10, 20);

    return 0;
}
```

The function was defined elsewhere and returned `double`, but no prototype was visible before the call.

**What I thought**

I assumed that because `calculate_average` was defined somewhere else in the project, the compiler would scan the whole codebase, find it automatically, figure out its signature, and handle the return value correctly.

**What actually happens**

In old C, calling a function without a visible declaration could make the compiler assume an implicit `int` return type.

In modern C, including C17, calling an undeclared function is not valid and should be diagnosed by the compiler.

The real lesson is the same in both cases: the caller must see a correct prototype before the function call.

Otherwise, the compiler cannot fully type-check the boundary between caller and callee.

**Fix**

```c
/* math_utils.h */
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

double calculate_average(int a, int b);

#endif
```

```c
/* main.c */
#include "math_utils.h"

int main(void)
{
    double result = calculate_average(10, 20);

    return 0;
}
```

**Lesson**

Never rely on the compiler discovering function signatures by scanning the project.

Always provide a visible prototype before calling a function, usually through a header.

## Problem 2 — Missing the second file during compilation

**What I wrote**

```bash
gcc main.c -o app
```

**What I thought**

I thought that because `main.c` contained:

```c
#include "flags.h"
```

the compiler and linker would automatically know where to find the implementation in `flags.c`.

**What actually happens**

The header gives the compiler declarations.

It does not compile or link the implementation.

If only `main.c` is compiled, the compiler can see the function prototypes, but the linker cannot find the actual function definitions from `flags.c`.

The result is an undefined-reference linker error.

**Fix**

```bash
gcc main.c flags.c -o app
```

For this repo, the stricter command is:

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic main.c flags.c -o demo
```

**Lesson**

A header file provides declarations for the compiler.

The relevant `.c` files must still be compiled and linked together.

## Problem 3 — Accidental symbol clash caused by missing `static`

**What I wrote**

```c
/* utils.c */
int validate_range(int value)
{
    return value >= 0 && value <= 100;
}
```

```c
/* main.c */
int validate_range(int value)
{
    return value == 42;
}
```

**What I thought**

I assumed that helper functions defined in different `.c` files lived in isolated worlds.

I thought common helper names like `validate_range` could be reused freely in different files.

**What actually happens**

Functions defined at file scope have external linkage by default.

When the linker combines `main.o` and `utils.o`, it sees two definitions of the same external symbol:

```text
validate_range
```

That creates a multiple-definition error.

**Fix**

If the helper belongs only to `utils.c`, make it private:

```c
/* utils.c */
static int validate_range(int value)
{
    return value >= 0 && value <= 100;
}
```

**Lesson**

Helper functions and module-internal routines should usually be marked `static`.

This gives them internal linkage and prevents global symbol collisions.

## Problem 4 — Reaching the end of a non-void function

**What I wrote**

```c
int get_status_code(int mode)
{
    if (mode == 1) {
        return 200;
    } else if (mode == 2) {
        return 404;
    }
}
```

**What I thought**

I thought that if the expected cases were covered, the function would naturally avoid falling off the end.

I also thought the compiler might implicitly return `0`.

**What actually happens**

If `mode` is neither `1` nor `2`, execution reaches the closing brace without a `return`.

The function promised to return an `int`, but that execution path returns no valid value.

If the caller uses the function call value on that path, the behavior is undefined.

**Fix**

```c
int get_status_code(int mode)
{
    if (mode == 1) {
        return 200;
    } else if (mode == 2) {
        return 404;
    }

    return 500;
}
```

**Lesson**

Every path in a non-void function that the caller may use must return a valid value.

## Problem 5 — Trying to bypass encapsulation with `extern`

**What I wrote**

```c
/* state.c */
static int internal_counter = 0;
```

```c
/* main.c */
extern int internal_counter;

void reset_counter(void)
{
    internal_counter = 0;
}
```

**What I thought**

I thought `extern` was a universal key that could reach into another source file and expose a variable, regardless of how it was declared there.

**What actually happens**

`extern` cannot bypass internal linkage.

Because `internal_counter` was marked `static` at file scope, it is private to `state.c`.

There is no external symbol named `internal_counter` for the linker to connect to.

The result is an undefined-reference error.

**Fix**

Expose controlled operations instead of exposing the variable directly.

```c
/* state.h */
#ifndef STATE_H
#define STATE_H

void reset_counter(void);
int counter_value(void);

#endif
```

```c
/* state.c */
#include "state.h"

static int internal_counter = 0;

void reset_counter(void)
{
    internal_counter = 0;
}

int counter_value(void)
{
    return internal_counter;
}
```

**Lesson**

`extern` cannot pierce internal linkage.

File-scope `static` strictly confines a symbol to its own translation unit.

## Problem 6 — Macro side effects and repeated evaluation

**What I wrote**

```c
#define SQUARE(x) ((x) * (x))

int main(void)
{
    int a = 5;
    int b = SQUARE(a++);

    return 0;
}
```

**What I thought**

I treated the macro like a function.

I expected `a++` to be evaluated once, just as it would be if passed to a normal function.

**What actually happens**

Macros perform textual substitution.

The expansion is effectively:

```c
int b = ((a++) * (a++));
```

The argument is evaluated twice.

This modifies `a` more than once in the same expression without proper sequencing, which produces undefined behavior.

Even a parenthesized macro can still be dangerous if it evaluates an argument more than once.

**Fix**

Use a function when the argument may have side effects.

```c
static int square(int x)
{
    return x * x;
}
```

Then call it in a controlled way:

```c
int a = 5;
int b = square(a);

a++;
```

**Lesson**

Macros are not functions.

Do not pass expressions with side effects to macros that may evaluate their arguments more than once.

## Problem 7 — Duplicate definitions caused by missing header guards

**What I wrote**

```c
/* config.h */
struct SystemConfig {
    int max_connections;
    int timeout;
};
```

```c
/* main.c */
#include "config.h"
#include "network.h" /* network.h also includes config.h */
```

**What I thought**

I thought the compiler would recognize that `config.h` contained the same structure definition and skip the duplicate automatically.

**What actually happens**

`#include` is textual insertion.

If `config.h` is included twice into the same translation unit, the compiler may see the same type definition twice.

That can produce a redefinition error.

**Fix**

```c
#ifndef CONFIG_H
#define CONFIG_H

struct SystemConfig {
    int max_connections;
    int timeout;
};

#endif
```

**Lesson**

Every header should be protected by a header guard.

Header guards prevent repeated processing of the same header inside one translation unit.

## Hidden invariants

* A function must be declared before the compiler can fully check calls to it.
* A declaration and a definition must describe the same function.
* Every non-void function must return a valid value on every path that the caller may use.
* Parameters are local objects initialized from argument values.
* Headers should expose declarations, not private implementation.
* `#include` copies text into a translation unit.
* Each `.c` file is compiled as a separate translation unit.
* The linker connects external symbols across compiled object files.
* File-scope `static` prevents external linkage.
* `extern` cannot access a symbol with internal linkage.
* Macros are expanded before the compiler checks C semantics.
* A macro may evaluate an argument more than once.

## Summary of corrections

| Problem | Correct model |
| --- | --- |
| Calling a function without a prototype | The caller must see a valid declaration before the call |
| Missing source files in compilation | Headers declare; `.c` files must still be compiled and linked |
| Symbol collision across files | Use file-scope `static` for private helper functions |
| Missing return path | Every used path in a non-void function must return a value |
| `extern` on `static` objects | `extern` cannot bypass internal linkage |
| Macro side effects | Macros are textual and may evaluate arguments more than once |
| Duplicate header inclusion | Use header guards to prevent repeated header processing |

## Final observation

What made Day 14 different from previous days is that programming in C stopped being confined inside one comfortable `main.c` file.

The dangerous bugs are no longer only expression-level mistakes or arithmetic slip-ups.

They now appear at boundaries:

* between declarations and definitions
* between headers and source files
* between public interfaces and private implementation
* between translation units
* between preprocessor text and compiler semantics
* between external linkage and internal linkage

Managing these boundaries requires discipline.

Day 14 changed the exercise from writing C syntax into designing C structure.
