# Problems — Day 16 (Pointers, Arrays, and Strings — Advanced)

## Problem 1 — Attempting to modify strings through an array of pointers

**What I wrote**

```c
const char *const pointer_commands[] = {
    "help",
    "status",
    "version"
};

pointer_commands[0][0] = 'H';
```

**What I thought**

I thought that `pointer_commands[0]` provided direct access to a mutable character buffer representing the word `"help"`.

I assumed I could modify the characters in place.

**What actually happens**

The elements of `pointer_commands` point to string literals.

String literals must not be modified.

Attempting to modify a string literal produces undefined behavior.

On many modern systems, string literals are placed in non-writable storage, so such an operation often crashes, but a segmentation fault is not guaranteed by C.

The declaration also uses:

```c
const char *const
```

which means both the pointed characters and the stored pointer values are treated as non-modifiable through this declaration.

**Fix**

Use writable character arrays when the contents need to change:

```c
char fixed_commands[][16] = {
    "help",
    "status",
    "version"
};

fixed_commands[0][0] = 'H';
```

**Lesson**

An array of pointers is not inherently read-only.

The important question is what the pointers point to and whether those objects are writable.

String literals must not be modified.

## Problem 2 — Confusing function-pointer syntax with function declarations

**What I wrote**

```c
int *handler(void) = run_status;
```

**What I thought**

I thought that:

```c
int *handler(void)
```

declared a pointer variable named `handler` that could store a function returning `int`.

**What actually happens**

This declaration describes a function named `handler`.

It says that the function:

```text
takes no arguments
returns int *
```

It does not declare a function-pointer variable.

The function declarator `()` binds differently unless the pointer declarator is grouped with parentheses.

**Fix**

```c
int (*handler)(void) = run_status;
```

The parentheses force:

```c
*handler
```

to be grouped before the function declarator.

This makes `handler` a pointer to a function that takes no arguments and returns `int`.

**Lesson**

Function-pointer declarations depend heavily on parentheses.

These two declarations are fundamentally different:

```c
int *handler(void);
```

```c
int (*handler)(void);
```

The first is a function returning `int *`.

The second is a pointer to a function returning `int`.

## Problem 3 — Misinterpreting `argv` as a flat character sequence

**What I wrote**

```c
char *cursor = argv;

while (*cursor != '\0') {
    /* attempting to scan all command-line arguments */
}
```

**What I thought**

I thought `argv` was one large character buffer containing the entire command line.

I expected a normal `char *` to traverse it character by character.

**What actually happens**

Inside `main`, `argv` has type:

```c
char **
```

Each element is itself a pointer to a null-terminated string.

Therefore:

```c
argv[0]
argv[1]
argv[2]
```

are separate `char *` values.

Assigning `argv` to `char *` is an incompatible pointer conversion and should be diagnosed by the compiler.

If the type mismatch is forced and the resulting pointer is then treated as character data, the program may interpret the representation of pointer objects as characters and invoke undefined behavior.

**Fix**

To walk across arguments:

```c
char **cursor = argv;
char **end = argv + argc;

while (cursor < end) {
    printf("%s\n", *cursor);
    cursor++;
}
```

To walk through characters inside one argument:

```c
char *cursor = argv[1];

while (*cursor != '\0') {
    printf("%c\n", *cursor);
    cursor++;
}
```

**Lesson**

The pointer level must match what is being traversed.

```text
char ** -> traverse argument pointers
char *  -> traverse characters inside one argument
```

## Problem 4 — Expecting `sizeof` to preserve array length in a function

**What I wrote**

```c
void process_names(const char *const items[])
{
    size_t count = sizeof items / sizeof items[0];

    /* use count */
}
```

**What I thought**

I assumed that passing an array into a function preserved its full array extent.

I expected `sizeof items` to produce the total size of the original array.

**What actually happens**

In a function parameter declaration:

```c
const char *const items[]
```

is adjusted to a pointer parameter.

Inside the function, `items` is therefore a pointer.

`sizeof items` gives the size of that pointer object, not the size of the original caller-side array.

The original number of elements is not carried automatically across the function boundary.

**Fix**

Pass the element count explicitly:

```c
void process_names(const char *const items[], size_t count)
{
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", items[i]);
    }
}
```

Calculate it where the real array still exists:

```c
size_t count = sizeof names / sizeof names[0];

process_names(names, count);
```

**Lesson**

An array parameter does not preserve the original array extent.

A function therefore needs an explicit length or another agreed termination convention.

## Problem 5 — Assuming table lookups always return valid pointers

**What I wrote**

```c
const struct Command *cmd =
    find_command(commands, count, "unknown_cmd");

int result = cmd->handler();
```

**What I thought**

I assumed that every table lookup would return a matching command entry.

I immediately dereferenced the returned pointer and called its handler.

**What actually happens**

If the requested name is not present, `find_command` returns:

```c
NULL
```

Dereferencing a null pointer through:

```c
cmd->handler
```

produces undefined behavior.

On many systems this commonly results in a segmentation fault, but C does not guarantee that particular outcome.

**Fix**

```c
const struct Command *cmd =
    find_command(commands, count, "unknown_cmd");

if (cmd == NULL) {
    printf("command not found\n");
} else {
    int result = cmd->handler();
    printf("result = %d\n", result);
}
```

**Lesson**

A lookup function that may return `NULL` establishes a contract.

The caller must verify success before accessing the returned object or invoking behavior through it.

## Hidden invariants

* String literals must not be modified.
* An array of pointers is not inherently read-only.
* `const char *const` protects both the pointed characters and the pointer object through that declaration.
* A pointer-to-pointer represents an additional level of indirection.
* `char **` and `char *` represent different traversal levels.
* Multi-dimensional arrays are not pointer-to-pointer objects.
* A pointer to an array and a pointer to pointer are different types.
* Function pointers must have compatible function types.
* Parentheses can completely change the meaning of a declaration.
* Array parameters do not preserve the original array extent.
* Lookup functions may return `NULL`.
* A null pointer must not be dereferenced.
* Function-pointer dispatch requires a successful lookup before invocation.

## Summary of corrections

| Problem | Correct model |
| --- | --- |
| Modifying string literals | String literals must not be modified |
| Function-pointer declaration | Use `int (*handler)(void)` for a pointer to a compatible function |
| Treating `argv` as `char *` | `argv` is a pointer-to-pointer; each element points to one string |
| Using `sizeof` on an array parameter | Array parameters are adjusted to pointers and lose array extent |
| Dereferencing failed lookup | Check for `NULL` before accessing the result or invoking its handler |

## Final observation

Day 16 introduced bugs involving not only where pointers point, but also how pointer levels and declarator structure represent relationships between objects.

A `char *`, a `char **`, a pointer to an array, and a function pointer are all pointer types, but they describe fundamentally different operations.

The key lesson is that advanced pointer code becomes manageable only when each level of indirection, each array boundary, and each function type is treated as an explicit contract.
