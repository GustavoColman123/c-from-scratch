# Notes — Pointers, Arrays, and Strings (Advanced)

## Source

K&R Chapter 5.

Sections covered:

* 5.6 Pointer Arrays; Pointers to Pointers
* 5.7 Multi-dimensional Arrays
* 5.8 Initialization of Pointer Arrays
* 5.9 Pointers vs Multi-dimensional Arrays
* 5.10 Command-line Arguments
* 5.11 Pointers to Functions
* 5.12 Complicated Declarations

## 1. Main idea

Day 16 moves beyond basic pointer traversal into organizing access to both data and behavior.

Pointers can be stored inside arrays, pointed to by other pointers, used to represent command-line argument vectors, and used to refer to compatible functions.

This allows C programs to build explicit tables that describe:

* where data is located
* how collections of objects are organized
* which function should execute for a given command or event

The central shift from Day 15 is:

```text
pointer traversal -> pointer-based organization and dispatch
```

Key concepts:

* Arrays of pointers store pointer values as their elements.
* Pointers to pointers allow traversal across arrays whose elements are themselves pointers.
* Multi-dimensional arrays contain nested array objects with fixed dimensions.
* Pointer arrays and multi-dimensional arrays have different types and layouts.
* `argc` and `argv` represent command-line input.
* Function pointers refer to compatible functions.
* Dispatch tables connect identifiers with executable behavior.
* Complex C declarations can combine pointers, arrays, and functions.

## 2. Arrays of string pointers

Example:

```c
const char *const names[] = {
    "kernel",
    "memory",
    "scheduler"
};
```

`names` is a one-dimensional array.

Each element has type:

```c
const char *const
```

Each element stores a pointer to the first character of a string literal.

The first `const` means the pointed characters should not be modified.

The second `const` means the pointer stored in each array element cannot be reassigned through this declaration.

Therefore, with this exact declaration, neither of these operations is allowed:

```c
names[0] = "boot";
```

```c
names[0][0] = 'K';
```

String literals themselves must not be modified.

Attempting to modify a string literal produces undefined behavior.

An implementation may place string literals in non-writable storage, but C does not require a specific physical memory region.

## 3. Pointers to pointers

Given:

```c
const char *const *cursor = names;
```

`cursor` points to an object whose type is:

```c
const char *const
```

Single indirection:

```c
*cursor
```

accesses the current pointer stored in the array.

Initially, it refers to:

```text
kernel
```

Double indirection:

```c
**cursor
```

accesses the first character of that string:

```text
k
```

After:

```c
cursor++;
```

`cursor` advances to the next element of the pointer array.

It now points to `names[1]`.

Therefore:

```c
*cursor
```

refers to:

```text
memory
```

and:

```c
**cursor
```

is:

```text
m
```

Pointer arithmetic advances according to the pointed type.

The concrete byte size of a pointer is implementation-dependent.

## 4. Multi-dimensional arrays

A true multi-dimensional array contains nested array objects.

Example:

```c
int matrix[3][4];
```

`matrix` is an array of 3 elements.

Each element has type:

```c
int[4]
```

Therefore:

```c
matrix[i][j]
```

first selects row `i`, then element `j` inside that row.

The complete storage belongs to one contiguous array object.

In most expressions, `matrix` is converted to a pointer to its first row.

Its resulting pointer type is:

```c
int (*)[4]
```

This is a pointer to an array of four `int`.

It is not:

```c
int **
```

A pointer to pointer and a pointer to an array are fundamentally different types.

## 5. Initialization of pointer arrays

Pointer arrays can be initialized directly with pointers to existing objects.

Example:

```c
const char *commands[] = {
    "help",
    "status",
    "version"
};
```

Each element is initialized with a pointer to the first character of a string literal.

The strings do not need to have equal lengths because the array stores pointer values rather than fixed-size character rows.

This differs from:

```c
char commands[][16] = {
    "help",
    "status",
    "version"
};
```

where every element of the outer array is a complete array of 16 characters.

## 6. Pointer arrays vs multi-dimensional arrays

These declarations describe different structures:

```c
const char *commands[3];
```

and:

```c
char commands[3][16];
```

The first creates:

```text
array of 3 pointers to const char
```

The second creates:

```text
array of 3 arrays of 16 char
```

The pointer-array version stores pointer values.

The objects being pointed to may live elsewhere and may have different lengths.

The fixed two-dimensional version owns all character storage directly.

Its rows have a fixed size and are contiguous.

For example:

```c
char fixed_commands[][16] = {
    "help",
    "status",
    "version"
};
```

contains three writable character arrays.

This is valid:

```c
fixed_commands[0][0] = 'H';
```

With:

```c
const char *commands[] = {
    "help",
    "status",
    "version"
};
```

the pointers refer to string literals, which must not be modified.

Pointer arrays themselves are not inherently read-only.

They may also point to writable arrays:

```c
char first[] = "help";
char second[] = "status";

char *commands[] = {
    first,
    second
};
```

The important distinction is the type and ownership of the pointed objects.

## 7. Command-line arguments

A hosted C program may define `main` as:

```c
int main(int argc, char *argv[])
```

In a function parameter declaration:

```c
char *argv[]
```

is adjusted to:

```c
char **argv
```

`argc` gives the number of argument strings available through `argv`.

`argv` provides access to those strings.

For example:

```bash
./demo help status
```

normally produces:

```text
argc = 3

argv[0] -> "./demo"
argv[1] -> "help"
argv[2] -> "status"
```

C also guarantees:

```c
argv[argc] == NULL
```

When `argc > 0`, `argv[0]` represents the program invocation name or an empty string if that information is unavailable.

Two different pointer levels are involved.

This:

```c
char **cursor = argv;
```

traverses argument pointers.

This:

```c
char *cursor = argv[1];
```

traverses characters inside one argument.

That distinction is a practical example of multiple levels of indirection.

## 8. Searching pointer tables

Pointer tables can be searched by comparing their pointed data.

Example:

```c
static const char *find_name(const char *const names[],
                             size_t count,
                             const char *target)
{
    for (size_t i = 0; i < count; i++) {
        if (string_equal(names[i], target)) {
            return names[i];
        }
    }

    return NULL;
}
```

If a match is found, the function returns a pointer to the matching string.

If no match exists, it returns:

```c
NULL
```

This establishes a contract between the lookup function and its caller.

The caller must check whether the returned pointer is `NULL` before using it.

A pointer-returning lookup makes failure explicit without requiring global error state.

## 9. Function pointers

A function pointer can refer to a compatible function.

Example:

```c
int (*handler)(void);
```

This declares `handler` as a pointer to a function that:

```text
takes no arguments
returns int
```

It can point to a compatible function such as:

```c
int run_status(void)
{
    return 0;
}
```

Assignment:

```c
handler = run_status;
```

The function can then be called through the pointer:

```c
handler();
```

This is also valid:

```c
(*handler)();
```

The shorter form is normally preferred.

Function-pointer type compatibility matters.

A pointer declared as:

```c
int (*handler)(void);
```

must only be used to call functions whose types are compatible with:

```c
int function_name(void);
```

## 10. Function declaration vs function-pointer declaration

These declarations look similar but mean different things.

```c
int *handler(void);
```

This declares a function named `handler`.

The function takes no arguments and returns:

```c
int *
```

In contrast:

```c
int (*handler)(void);
```

declares a variable named `handler`.

The variable is a pointer to a function returning `int`.

The parentheses are essential.

Without them, the declaration has a completely different structure.

## 11. Complicated declarations

Pointers, arrays, and functions can be combined into declarations that are difficult to read.

Examples:

```c
int *f(void);
```

Function returning pointer to `int`.

```c
int (*f)(void);
```

Pointer to function returning `int`.

```c
char *argv[];
```

Array of pointers to `char`.

```c
char (*p)[16];
```

Pointer to array of 16 `char`.

```c
int (*handlers[4])(void);
```

Array of four pointers to functions that take no arguments and return `int`.

A useful strategy is to begin at the identifier and work outward, respecting parentheses and declarator precedence.

For:

```c
int (*handlers[4])(void);
```

start with:

```text
handlers
```

Then:

```text
handlers[4]
```

means an array of four.

The elements are pointers:

```text
*handlers[4]
```

Those pointers refer to functions taking no arguments:

```text
(*handlers[4])(void)
```

Finally, the functions return:

```text
int
```

## 12. Dispatch tables

A structure can connect a string identifier to a function pointer.

Example:

```c
struct Command {
    const char *name;
    int (*handler)(void);
};
```

A table can then be created:

```c
const struct Command commands[] = {
    {"help", run_help},
    {"status", run_status},
    {"version", run_version}
};
```

Each entry represents:

```text
command name -> compatible function
```

This turns behavior selection into data.

Instead of writing a long chain of conditionals, the program can search the table and invoke the stored handler.

Adding another command mostly requires adding another table entry rather than extending the central branching logic.

## 13. Mini shell-style dispatch

A lookup function can return a pointer to a matching command structure:

```c
const struct Command *command =
    find_command(commands, count, name);
```

The result must be checked:

```c
if (command == NULL) {
    return 1;
}
```

When the lookup succeeds:

```c
return command->handler();
```

performs an indirect function call.

This combines several Day 16 concepts:

```text
string input
    ↓
pointer-table search
    ↓
pointer to matching structure
    ↓
function pointer
    ↓
indirect function call
```

The integer returned by the handler can act as a status code.

## 14. Connection to NucleOS

Day 16 is directly relevant to future NucleOS architecture.

A small shell could represent commands as:

```c
struct Command {
    const char *name;
    int (*handler)(void);
};
```

The parser could search a static command table and invoke the corresponding handler.

For example:

```text
help    -> run_help
status  -> run_status
version -> run_version
```

Similar dispatch-table ideas can also appear in:

* driver operation tables
* filesystem operation tables
* parser tables
* state machines
* architecture-specific handler tables
* simplified interrupt or syscall dispatch structures

Real operating-system implementations introduce additional concerns such as ABI rules, privilege boundaries, concurrency, architecture-specific entry code, and validation.

Day 16 does not implement those systems.

It introduces the underlying idea of representing behavior through typed function-pointer tables.

## Practical takeaways

1. Arrays can store pointers as elements.
2. A pointer to pointer adds another level of indirection.
3. `*p` and `**p` access different levels of a pointer structure.
4. Pointer arithmetic advances according to the pointed type.
5. Multi-dimensional arrays are arrays whose elements are arrays.
6. A pointer to pointer is not the same type as a pointer to an array.
7. Pointer arrays and fixed multi-dimensional arrays have different layouts.
8. Pointer arrays are not inherently read-only.
9. In a function parameter, `char *argv[]` is adjusted to `char **argv`.
10. `argv` provides a sequence of pointers to argument strings.
11. `argv[argc]` is a null pointer sentinel.
12. Function pointers refer to compatible functions.
13. Parentheses are essential in many function-pointer declarations.
14. Lookup functions may use `NULL` to represent failure.
15. Dispatch tables map data identifiers to executable behavior.
16. Complicated declarations should be read from the identifier outward.

## Final observation

Day 16 bridged low-level pointer mechanics with architectural organization.

Day 15 used pointers primarily to access and traverse objects.

Day 16 used pointers to organize collections of addresses and represent executable behavior.

By combining pointer arrays, multiple levels of indirection, command-line arguments, function pointers, and dispatch tables, C can express data relationships and runtime behavior through explicit structures.

These patterns form a useful foundation for future NucleOS components such as command dispatch, driver tables, parsers, and other modular low-level interfaces.
