# Notes — Functions and Program Structure

## Source

K&R Chapter 4.

Sections covered:

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

## 1. Main idea

Functions in C are not just reusable blocks of code.

They are boundaries between:

* caller and callee
* declaration and definition
* public interface and private implementation
* automatic local state and external state
* source code and preprocessing

A function establishes a contract between the code that calls it and the code that implements it.

The caller provides argument values.

The callee receives those values through parameters, performs its work, and may return a value to the caller.

This makes functions one of the main tools for structuring C programs.

## 2. Function definitions

A function definition provides the actual implementation of a routine.

Example:

```c
int add(int a, int b)
{
    return a + b;
}
```

The parts are:

* return type: `int`
* function name: `add`
* parameters: `int a, int b`
* function body: the block between braces
* return statement: `return a + b;`

The definition provides the body that the compiler can translate into executable code.

## 3. Caller and callee

When one function invokes another, control moves from the caller to the callee.

Example:

```c
int result = add(3, 4);
```

The caller is the function containing the call.

The callee is `add`.

The callee receives the argument values `3` and `4` through its parameters.

It computes a result and returns the value `7`.

Then control returns to the caller, and the function call expression produces the returned value.

## 4. Parameters are passed by value

In C, arguments are passed by value.

Example:

```c
void change(int x)
{
    x = 99;
}
```

Changing `x` inside `change` modifies only the local parameter object.

It does not modify the caller's original object.

A pointer can be passed to a function, but the pointer value itself is still passed by value.

This distinction becomes important when functions need to modify objects owned by the caller.

## 5. Return values

A `return` statement exits the current function call.

Example:

```c
int max(int a, int b)
{
    if (a > b)
        return a;

    return b;
}
```

When `return` is executed:

* control returns to the caller
* the returned expression becomes the value of the function call
* the lifetimes of ordinary automatic local objects in that function end
* the returned expression is converted to the declared return type

Example:

```c
int f(void)
{
    return 3.7;
}
```

This returns `3`.

The floating-point value `3.7` is converted to `int`, which discards the fractional part.

This kind of implicit loss of precision should usually be avoided.

## 6. Missing return in non-void functions

This function is broken:

```c
int broken(int x)
{
    if (x > 0)
        return x;
}
```

The function promises to return an `int`.

However, if `x <= 0`, execution reaches the end of the function without returning a value.

If the caller uses the value of that function call, the behavior is undefined.

A non-void function should return a valid value on every path that the caller may use.

## 7. Declarations and definitions

A declaration tells the compiler the interface of a function.

Example:

```c
int add(int a, int b);
```

This is a prototype.

It says that `add` takes two `int` arguments and returns an `int`.

A definition provides the actual body:

```c
int add(int a, int b)
{
    return a + b;
}
```

Declarations allow the compiler to check calls before the body is seen.

Definitions provide the implementation.

A mismatch between declaration and definition breaks the function-call contract.

## 8. Header files

A header file exposes the public interface of a module.

For this exercise, the program is divided into:

```text
main.c
flags.c
flags.h
```

The role of each file is:

* `flags.h` exposes public declarations and symbolic constants
* `flags.c` contains the implementation and private module state
* `main.c` uses the module through the public interface

This separates interface from implementation.

The caller does not need to know how the module stores its internal state.

It only needs to know which functions it may call.

## 9. Translation units and `#include`

The directive:

```c
#include "flags.h"
```

performs textual inclusion before compilation.

The preprocessor inserts the contents of `flags.h` into the source file at that point.

Each `.c` file plus the headers included into it becomes a separate translation unit.

For example:

```text
main.c + included headers  -> translation unit
flags.c + included headers -> translation unit
```

These translation units are compiled separately.

The linker then connects the compiled object files into one executable.

`#include` does not work like a Python import.

It copies text into the current translation unit before the compiler checks C semantics.

## 10. Header guards

A header guard prevents the same header from being processed more than once in a single translation unit.

Example:

```c
#ifndef FLAGS_H
#define FLAGS_H

/* declarations */

#endif
```

This avoids repeated definitions of types, macros, and other header contents that cannot safely appear more than once.

Header guards make headers safe to include through multiple paths.

## 11. External variables and `extern`

A variable defined outside any function has file scope.

Example:

```c
int counter = 0;
```

It also has static storage duration, which means the object exists for the entire execution of the program.

Unless restricted with `static`, a file-scope object normally has external linkage.

That means another translation unit may refer to it through an `extern` declaration:

```c
extern int counter;
```

An `extern` declaration says that the object exists elsewhere.

It usually does not create storage by itself.

External variables can be useful for shared state, but they are dangerous because they create hidden dependencies between pieces of code.

## 12. `static` at file scope

At file scope, `static` gives a name internal linkage.

Example:

```c
static unsigned flags = 0;
```

This means `flags` can be named only inside the translation unit where it is defined.

In this exercise, `flags` belongs privately to `flags.c`.

`main.c` cannot access it directly.

This protects the internal state of the module and forces outside code to use the public functions declared in `flags.h`.

## 13. Static helper functions

A helper function can also be marked `static`.

Example:

```c
static int valid_bit(unsigned bit)
{
    return bit < FLAGS_MAX_BITS;
}
```

This function is private to one `.c` file.

It is part of the implementation, not the public interface.

This avoids exposing implementation details, prevents symbol conflicts, and keeps the module interface small.

A useful rule is:

```text
If a function is only used inside one .c file, make it static.
```

## 14. Initialization

Objects with static storage duration are zero-initialized automatically.

Example:

```c
int global_counter;
```

If this appears at file scope, it is initialized to zero.

An automatic local variable is different:

```c
void f(void)
{
    int local_counter;
}
```

`local_counter` has an indeterminate value until explicitly initialized.

Reading an uninitialized automatic object produces undefined behavior.

## 15. Recursion

A recursive function calls itself.

Example:

```c
int factorial(int n)
{
    if (n <= 1)
        return 1;

    return n * factorial(n - 1);
}
```

The base case is:

```c
if (n <= 1)
    return 1;
```

The recursive case is:

```c
return n * factorial(n - 1);
```

A recursive function needs a base case to stop.

Each recursive call needs its own execution state.

In systems programming, recursion can be dangerous if the depth is unbounded, because stack space is limited.

For kernel or embedded code, recursion should be avoided unless the maximum depth is strictly bounded and understood.

## 16. Preprocessor basics

A symbolic constant can be created with `#define`:

```c
#define FLAGS_MAX_BITS 8u
```

This performs textual substitution before compilation.

It does not create storage.

The identifier itself does not have a C type.

Macros must be used carefully.

Example:

```c
#define SQUARE(x) ((x) * (x))
```

The parentheses help with precedence, but the argument is still evaluated twice.

This is dangerous with side effects:

```c
int y = SQUARE(i++);
```

Macros are not functions.

They are text transformations performed before C semantic analysis.

Conditional compilation can include or exclude code before compilation:

```c
#ifdef DEBUG
    printf("debug mode\n");
#endif
```

This is useful for debug builds, feature flags, architecture-specific code, and experimental systems code.

## 17. Connection to NucleOS

This exercise is indirectly connected to NucleOS because operating-system code must be modular.

A kernel cannot be one giant `main.c`.

It needs modules that expose safe public operations while hiding internal state.

In this exercise:

```c
static unsigned flags = 0;
```

is private module state.

And:

```c
int flags_set(unsigned bit);
```

is public module behavior.

This pattern can later appear in schedulers, memory managers, device drivers, command parsers, and kernel state modules.

A public header exposes what other files are allowed to use.

A `.c` file owns the implementation.

`static` prevents accidental cross-file access.

Return values report success or failure instead of allowing invalid state changes.

## Practical takeaways

1. A function declaration is a contract.
2. A function definition is an implementation.
3. Parameters are passed by value.
4. Return values cross the caller/callee boundary.
5. Headers expose public interfaces.
6. `.c` files contain implementations.
7. `#include` is textual inclusion.
8. Each `.c` file becomes a separate translation unit after preprocessing.
9. The linker connects compiled object files.
10. File-scope `static` gives internal linkage.
11. External state should be minimized.
12. The preprocessor transforms source before compilation.

## Final observation

Building the first multi-file exercise shifted my mental model from viewing C as isolated single-file programs to understanding it as an engineered system of modules, interfaces, translation units, and carefully controlled visibility.

Day 14 showed that writing C is not only about writing correct statements.

It is also about designing boundaries.
