# Day 15 — Pointers, Arrays, and Strings

## What I built

A single-file C program demonstrating the foundational mechanics of pointers, arrays, and strings.

The program covers:

* address-of operator
* indirection operator
* pointer declarations
* wrong swap vs pointer-based swap
* output pointers
* arrays and pointer arithmetic
* one-past pointers
* subarray views
* character arrays vs string literals
* string traversal
* string copy
* string comparison
* pointer-based search

## What changed from Day 14

Day 14 focused on program boundaries across functions, headers, source files, and translation units.

Day 15 moved into memory access through typed addresses.

The central shift was:

```text
program structure boundaries -> indirect access to objects
```

Day 14 was about designing modules.

Day 15 was about understanding how C reaches objects through addresses.

## What I built in code

### Section 1 — Address and Indirection

This section demonstrates:

```c
int x = 10;
int *p = &x;
```

`p` stores the address of `x`.

`*p` reads the value of `x`.

Assigning through `*p` modifies `x` indirectly.

The pointer value and the pointed object are different things.

### Section 2 — Swap and Pass-by-Address

`wrong_swap` fails because C passes arguments by value.

The function receives copies of `a` and `b`, not the original objects.

`swap_values` works because it receives pointer values.

Those pointer values are copies, but they still point to the caller's original objects.

This lets the function modify caller-owned data through indirection.

### Section 3 — Output Pointer

`read_constant` uses its return value to report success or failure.

It uses an output pointer argument:

```c
int *out
```

to write produced data into caller-owned storage.

The function checks for `NULL` before writing through the pointer.

This pattern separates operation status from produced data.

### Section 4 — Arrays and Pointer Arithmetic

This section demonstrates the equivalence between:

```c
values[i]
```

and:

```c
*(values + i)
```

It also shows a pointer walk using `p++`.

A one-past pointer is used as an end marker.

The one-past pointer is created and compared, but never dereferenced.

### Section 5 — Subarray View

Passing:

```c
values + 2
```

creates a view into the original array starting at `values[2]`.

No new array is created.

No copy is made.

The pointer simply starts at a later element in the same array.

### Section 6 — Character Arrays and String Literals

This section separates two ideas:

```c
char message_array[] = "now is the time";
```

is a modifiable character array.

```c
const char *message_literal = "now is the time";
```

points to a string literal that must not be modified.

This distinction matters because strings in C are arrays of characters terminated by `'\0'`, but not every character sequence is writable.

### Section 7 — String Traversal

This section compares index-based and pointer-based string length functions.

Both scan until `'\0'`.

The pointer-based version moves through the string with `s++`.

Pointer subtraction gives the distance between the start and the end of the string.

### Section 8 — String Copy and Compare

`copy_string` uses assignment inside a `while` loop:

```c
while ((*dest++ = *src++) != '\0') {
    ;
}
```

The assignment copies one character and produces the copied value.

The loop stops after copying the null terminator.

`compare_string` compares characters until it finds a difference or reaches `'\0'`.

The sign of the return value matters:

* `0` means equal
* negative means the first string is smaller
* positive means the first string is greater

### Section 9 — Pointer Search

`find_char` scans a string and returns a pointer to the matching character.

If the character is not found, it returns `NULL`.

When a match is found, pointer subtraction can compute the offset from the beginning of the string.

This works only because both pointers refer into the same string object.

## Example output

```text
=== Day 15: Pointers, Arrays, and Strings ===

=== Section 1: Address and Indirection ===

x before = 10
*p reads = 10
p points to x = yes
x after *p = 99 -> 99

=== Section 2: Swap and Pass-by-Address ===

before wrong_swap: a = 3, b = 7
after wrong_swap:  a = 3, b = 7
after swap_values: a = 7, b = 3

=== Section 3: Output Pointer ===

read_constant succeeded: output = 42
read_constant rejected a null output pointer

=== Section 4: Arrays and Pointer Arithmetic ===

values: 10 20 30 40 50
sum using index   = 150
sum using pointer = 150
values[2]       = 30
*(values + 2)   = 30
pointer walk: 10 20 30 40 50
one-past pointer created safely = yes

=== Section 5: Subarray View ===

subarray[0] points to original values[2] = 30
subarray sum from values + 2 = 120

=== Section 6: Character Arrays and String Literals ===

modifiable array before = now is the time
modifiable array after  = Now is the time
string literal view     = now is the time

=== Section 7: String Traversal ===

text = kernel
length using index   = 6
length using pointer = 6

=== Section 8: String Copy and Compare ===

copied string = kernel flags
compare("abc", "abc") = 0
compare("abc", "abd") = -1
compare("abd", "abc") = 1

=== Section 9: Pointer Search ===

first 'l' offset in bootloader = 4
'x' was not found in bootloader
```

## What clicked

### Pointers are typed address values

Pointers are not magic and they are not just raw untyped numbers.

A pointer value identifies an object, and its type tells C how that object should be accessed.

### `*p` means access the pointed object

The expression `p` refers to the pointer object.

The expression `*p` refers to the object reached through the pointer.

This distinction controls whether code modifies the pointer itself or the object it points to.

### C still passes by value

Passing a pointer does not contradict pass-by-value.

The address value is copied into the function parameter.

The copy still points to the caller's object, so the callee can modify that object through indirection.

### Arrays decay to pointers in most expressions

An array is not a pointer variable.

However, in most expressions, the array expression becomes a pointer to its first element.

This explains why array indexing and pointer arithmetic are so closely related.

### Pointer arithmetic moves by elements

Adding `1` to a pointer moves to the next object of the pointed type.

It does not simply add one raw byte unless the pointer type is `char *`.

This makes array traversal possible.

### Strings are null-terminated arrays of characters

C strings depend on the `'\0'` terminator.

String traversal works only when that terminator exists.

Without it, a function may continue reading past the intended buffer.

### Buffers require explicit limits

C does not automatically know how much writable space a destination buffer has.

String copying is safe only when the destination is large enough and the source is properly terminated.

## Mistakes / confusions

* I had to stop thinking of pointers as raw untyped numbers.
* I had to distinguish the pointer object from the object it points to.
* I had to remember that C is still pass-by-value when passing pointers.
* I had to understand that arrays are not pointer variables.
* I had to remember that array parameters lose size information.
* I had to stop thinking that one-past pointers can be dereferenced.
* I had to separate modifiable character arrays from string literals.
* I had to remember that C strings need a `'\0'` terminator.
* I had to be careful with destination buffer size.
* I had to understand that `*p++` increments the pointer, not the pointed object.

## Connection to NucleOS

This exercise is directly connected to future NucleOS work because operating-system code constantly deals with buffers, tables, strings, and addresses.

A command parser will scan character buffers.

A scheduler will walk task control blocks or task tables.

A memory manager will manipulate address ranges.

Drivers may read and write through pointers to memory-like regions.

Day 15 introduces the foundational mechanics required for low-level systems engineering:

* typed addresses
* indirect access
* traversal
* bounds
* null checks
* string termination
* writable buffers
* explicit ownership
* object lifetime

The exercise is small, but the mechanics are the same ones that future systems code will depend on.

## Final reflection

Day 15 showed that pointers are the point where C becomes serious.

They make memory traversal, string processing, array access, and output parameters possible, but they also make invalid addresses, missing bounds, and object lifetime unavoidable.

The main lesson is that C gives direct control over how objects are reached and traversed.

That control is powerful, but it only remains safe when the pointer is valid, the bounds are known, the destination is writable, and the object's lifetime has not ended.

## Time

4 hours — reading, question review, implementation, output testing, problems, notes, and documentation.
