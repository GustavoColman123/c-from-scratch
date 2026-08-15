# Problems — Day 15 (Pointers, Arrays, and Strings)

## Problem 1 — Modifying local parameters in `swap`

**What I wrote**

```c
void wrong_swap(int x, int y)
{
    int temp = x;

    x = y;
    y = temp;
}
```

**What I thought**

I thought that passing variables by name into a function would allow the function to manipulate and swap the caller's actual variables directly.

**What actually happens**

C passes arguments by value.

The function receives local parameter objects initialized with copies of the caller's values.

The swap happens only between `x` and `y`.

Those are local to `wrong_swap`.

When the function returns, the caller's original variables are unchanged.

**Fix**

```c
void swap_values(int *px, int *py)
{
    int temp = *px;

    *px = *py;
    *py = temp;
}
```

Call it with addresses:

```c
swap_values(&a, &b);
```

**Lesson**

To modify caller-owned objects, pass their addresses and dereference those pointers inside the callee.

## Problem 2 — Forgetting the address-of operator in function calls

**What I wrote**

```c
int a = 3;
int b = 7;

swap_values(a, b);
```

**What I thought**

I assumed that because the function expected pointers, the compiler would automatically take the addresses of `a` and `b`.

**What actually happens**

The call does not match the function prototype.

The function expects arguments of type `int *`, but `a` and `b` have type `int`.

A modern compiler should diagnose this mismatch.

If such a call is forced through casts or ignored warnings, the function may receive invalid pointer values.

Dereferencing invalid pointer values produces undefined behavior.

**Fix**

```c
swap_values(&a, &b);
```

**Lesson**

When a function expects a pointer to an object, explicitly pass the object's address with `&`.

## Problem 3 — Confusing the pointer with the pointed object

**What I wrote**

```c
int x = 10;
int *p = &x;

p = 99;
```

**What I thought**

I treated `p` and `*p` as if they were interchangeable.

I thought assigning `99` to `p` would update the value of `x`.

**What actually happens**

`p` is the pointer object.

`*p` is the object pointed to by `p`.

Assigning an integer to a pointer is not a valid way to modify the pointed object.

A compiler should warn about or reject this conversion.

If such a value is forced into a pointer, the pointer may contain an invalid or implementation-defined pointer value.

Dereferencing it would produce undefined behavior.

**Fix**

```c
*p = 99;
```

**Lesson**

The pointer variable holds an address.

To modify the target object, dereference the pointer.

## Problem 4 — Dereferencing an uninitialized pointer

**What I wrote**

```c
int *p;

*p = 42;
```

**What I thought**

I declared a pointer and immediately assigned a value to its destination.

I assumed it pointed to some valid default location.

**What actually happens**

An uninitialized automatic pointer has an indeterminate value.

It does not necessarily point to any valid object.

Dereferencing it produces undefined behavior.

The program may crash, corrupt memory, or appear to work by accident.

**Fix**

```c
int x;
int *p = &x;

*p = 42;
```

**Lesson**

A pointer must contain a valid address before it is dereferenced.

## Problem 5 — Attempting to modify a string literal

**What I wrote**

```c
char *s = "kernel";

s[0] = 'K';
```

**What I thought**

I treated a string literal like a normal modifiable character array.

**What actually happens**

A string literal must not be modified.

Attempting to modify it produces undefined behavior.

On many modern systems, this often crashes, but the formal C rule is undefined behavior.

**Fix**

Use a modifiable array:

```c
char s[] = "kernel";

s[0] = 'K';
```

Or, if the string should not be modified, express that with `const`:

```c
const char *s = "kernel";
```

**Lesson**

Use `char array[]` for modifiable strings.

Use `const char *` for string literals that should not be modified.

## Hidden invariants

* A pointer must contain a valid address before it is dereferenced.
* `&x` requires an addressable object.
* `*p` accesses the object pointed to by `p`.
* Assigning to `*p` writes through the pointer.
* C passes pointer values by value.
* Array expressions often decay to pointers to their first element.
* Array parameters do not preserve array size.
* Pointer arithmetic is valid only within the same array object or one past it.
* A one-past pointer may be formed and compared, but not dereferenced.
* A C string must be null-terminated.
* String literals must not be modified.
* Destination buffers must be large enough before copying into them.
* `NULL` is a sentinel pointer value, not a valid object to dereference.
* `*p++` increments the pointer, not the pointed object.
* Returned pointers must not outlive the objects they point into.

## Summary of corrections

| Problem | Correct model |
| --- | --- |
| Modifying local parameters | C passes arguments by value; use pointers to modify caller-owned objects |
| Missing `&` in calls | Pass addresses explicitly when a function expects pointers |
| Confusing pointer with target | Use `*p = value` to modify the pointed object |
| Uninitialized pointer access | Initialize pointers to valid objects before dereferencing |
| Modifying string literals | String literals must not be modified; use `char array[]` for writable strings |

## Final observation

Day 15 introduced the point where C becomes genuinely low-level.

The bugs are no longer only about syntax, operators, or control flow.

They now involve invalid addresses, missing bounds, lost array sizes, string termination, and whether a function is allowed to modify caller-owned objects.

Navigating these mechanics requires shifting from abstract variable handling to spatial awareness of objects, addresses, bounds, and lifetimes.
