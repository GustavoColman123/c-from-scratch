# Notes — Pointers, Arrays, and Strings

## Source

K&R Chapter 5.

Sections covered:

* 5.1 Pointers and Addresses
* 5.2 Pointers and Function Arguments
* 5.3 Pointers and Arrays
* 5.4 Address Arithmetic
* 5.5 Character Pointers and Functions

## 1. Main idea

A pointer is a typed value that stores the address of an object.

Through indirection, C code can read or modify objects indirectly, traverse arrays, scan strings, and give functions controlled access to caller-owned data.

Key concepts:

* Objects are regions of storage in the C abstract machine that can hold values.
* Addresses are values that identify where objects can be accessed.
* Pointer types describe what kind of object a pointer is expected to point to.
* Indirection means accessing an object through a pointer instead of through the object's direct name.
* Arrays are contiguous sequences of elements that can be accessed through indexing or pointer traversal.
* Strings are character sequences terminated by a null character, `'\0'`.
* Function arguments are passed by value, but one of the values that can be passed is an address.
* Memory-like traversal in C is performed through pointer arithmetic, explicit bounds, and careful lifetime management.

## 2. Pointers and addresses

Example:

```c
int x = 10;
int *p = &x;
```

`x` is an object of type `int`.

`p` is an object of type `int *`.

The expression `&x` produces the address of `x`.

The pointer `p` stores that address.

When `p` stores the address of `x`, we say that `p` points to `x`.

## 3. The address-of operator `&`

The address-of operator produces the address of an object or function.

Example:

```c
int x = 10;
int *p = &x;
```

`&x` is valid because `x` designates an addressable object.

This is invalid:

```c
int *p = &(x + 1);
```

The expression `x + 1` produces a computed value.

It does not designate an addressable object.

The address-of operator requires something whose address can actually be taken.

## 4. The indirection operator `*`

The indirection operator accesses the object pointed to by a pointer.

Example:

```c
int x = 10;
int *p = &x;

printf("%d\n", *p);

*p = 99;
```

The expression `*p` accesses the object pointed to by `p`.

Since `p` points to `x`, reading `*p` reads `x`.

Assigning to `*p` modifies `x`.

The pointer value stored in `p` does not change.

Only the pointed object changes.

## 5. Pointer declarations and types

K&R's core idea is that a declaration mirrors the expression that uses the object.

Example:

```c
int *ip;
```

This means that `*ip` has type `int`.

Therefore, `ip` is a pointer to `int`.

Compare:

```c
int x;
```

and:

```c
int *p;
```

`x` stores an integer directly.

`p` stores the address of an integer.

Pointer types matter because they determine:

* how the pointed object is interpreted
* how dereferencing behaves
* how assignment through the pointer behaves
* how pointer arithmetic advances through memory-like sequences

## 6. Pointer expressions and precedence

The following expressions are different:

```c
y = *ip + 1;
```

This reads the pointed object, adds `1`, and stores the result in `y`.

It does not modify the pointed object.

```c
*ip += 1;
```

This modifies the pointed object.

It is equivalent to:

```c
*ip = *ip + 1;
```

```c
++*ip;
```

This is equivalent to:

```c
++(*ip);
```

It increments the pointed object.

```c
(*ip)++;
```

This also increments the pointed object, but as a post-increment expression.

The expression produces the old value first, then increments the object.

This expression is different:

```c
*ip++;
```

It is parsed as:

```c
*(ip++);
```

The pointer `ip` is incremented.

The pointed object is not incremented.

This expression is common in string and array traversal, but it must be read carefully.

## 7. Pointers and function arguments

This function does not modify the caller's variables:

```c
void wrong_swap(int x, int y)
{
    int temp;

    temp = x;
    x = y;
    y = temp;
}
```

C passes arguments by value.

The parameters `x` and `y` are local parameter objects initialized from copies of the caller's values.

Swapping those local objects does not affect the caller's original objects.

## 8. Pass-by-address with pointers

This version works:

```c
void swap_values(int *px, int *py)
{
    int temp;

    temp = *px;
    *px = *py;
    *py = temp;
}
```

Call:

```c
swap_values(&a, &b);
```

C still passes arguments by value.

The copied values are the addresses of `a` and `b`.

Inside the function, `px` points to `a` and `py` points to `b`.

The expressions `*px` and `*py` access the caller-owned objects indirectly.

This allows the function to modify `a` and `b`.

A precise model is:

```text
C is always pass-by-value, but one of the values you can pass is an address.
```

## 9. Output pointers

A function can use its return value for status and a pointer argument for produced data.

Example:

```c
int read_constant(int *out)
{
    if (out == NULL) {
        return 0;
    }

    *out = 42;
    return 1;
}
```

The return value reports success or failure.

The pointer argument provides an output location.

This separates operation status from produced data.

Before writing through an output pointer, the function must make sure the pointer is valid for that operation.

In this example, `NULL` is rejected before dereferencing.

## 10. Arrays and pointer behavior

An array is not a pointer object.

Example:

```c
int values[5];
```

`values` is an array object.

However, in most expressions, an array expression is converted to a pointer to its first element.

Therefore:

```c
values
```

often behaves like:

```c
&values[0]
```

Important exceptions include:

```c
sizeof values
```

and:

```c
&values
```

`sizeof values` gives the size of the whole array.

`&values` gives the address of the whole array object, not the same type as `&values[0]`.

## 11. Indexing and pointer arithmetic

Array indexing is defined in terms of pointer arithmetic.

These two expressions access the same element:

```c
values[i]
```

and:

```c
*(values + i)
```

The expression `values + i` points to the i-th element.

Pointer arithmetic moves by elements of the pointed type, not by raw bytes.

If `values` is an array of `int`, then `values + 1` points to the next `int`.

If a pointer points to `char`, then incrementing it moves to the next `char`.

## 12. Array parameters

As function parameters, these declarations are equivalent:

```c
void f(int arr[])
```

and:

```c
void f(int *arr)
```

The array is not passed by value.

The parameter becomes a pointer to the first element.

The function loses the original array size.

For safe traversal, the length must usually be passed separately:

```c
void f(int arr[], size_t length)
```

or:

```c
void f(int *arr, size_t length)
```

## 13. Subarray views

These calls both pass a pointer to the third element:

```c
f(&values[2]);
```

```c
f(values + 2);
```

No new array is created.

No copy is made.

The function receives a pointer into the original array.

From the function's perspective, that position becomes the starting point.

For example, if `p` receives `values + 2`, then:

```text
p[0] refers to values[2]
p[1] refers to values[3]
p[2] refers to values[4]
```

Bounds still matter.

The caller and callee must agree on how many elements may be accessed.

## 14. Address arithmetic and one-past pointers

A common traversal pattern is:

```c
const int *p = values;
const int *end = values + length;

while (p < end) {
    sum += *p;
    p++;
}
```

`p++` advances to the next element of the pointed type.

`end` points one past the last valid element.

A one-past pointer may be formed and compared.

It must not be dereferenced.

This is valid:

```c
p < end
```

This is not valid when `end` is one past the array:

```c
*end
```

A one-past pointer is a boundary marker, not a valid element.

## 15. Character arrays and string literals

These declarations are different:

```c
char message_array[] = "now is the time";
```

```c
const char *message_literal = "now is the time";
```

`message_array` is a modifiable character array initialized with the characters of the string literal, including the terminating `'\0'`.

`message_literal` points to a string literal.

String literals must not be modified.

Attempting to modify a string literal produces undefined behavior.

Using `const char *` expresses the intention that the pointed characters should not be modified.

## 16. String traversal

A C string is a sequence of characters terminated by `'\0'`.

An index-based length function can be written as:

```c
size_t length_index(const char s[])
{
    size_t length = 0;

    while (s[length] != '\0') {
        length++;
    }

    return length;
}
```

A pointer-based version can be written as:

```c
size_t length_pointer(const char *s)
{
    const char *start = s;

    while (*s != '\0') {
        s++;
    }

    return (size_t)(s - start);
}
```

Both versions scan until the null terminator.

Indexing and pointer traversal are two views of the same underlying sequence.

Pointer subtraction gives the distance between two pointers into the same array object.

The result of pointer subtraction has type `ptrdiff_t`.

In this exercise, the result is cast to `size_t` only after subtracting pointers within the same string.

`size_t` is the standard unsigned type used for sizes and lengths.

## 17. String copy

A compact string-copying loop is:

```c
while ((*dest++ = *src++) != '\0') {
    ;
}
```

The assignment copies one character from `src` to `dest`.

The assignment expression produces the assigned value.

Both pointers advance after each copy.

The loop stops after copying the null terminator.

This idiom depends on important preconditions:

* `dest` must point to writable storage.
* `dest` must have enough space.
* `src` must point to a valid null-terminated string.

Without those conditions, copying strings is dangerous.

## 18. String comparison

A pointer-based string comparison function can be written as:

```c
int compare_string(const char *s, const char *t)
{
    for (; *s == *t; s++, t++) {
        if (*s == '\0') {
            return 0;
        }
    }

    return (unsigned char)*s - (unsigned char)*t;
}
```

The function returns `0` when the strings are equal.

It returns a negative value when the first differing character in `s` is smaller than the corresponding character in `t`.

It returns a positive value when the first differing character in `s` is greater.

Only the sign matters, not the exact number.

Casting to `unsigned char` avoids surprises from implementations where plain `char` is signed.

## 19. Pointer search

A search function can return a pointer into the original string:

```c
const char *find_char(const char *s, char target)
{
    while (*s != '\0') {
        if (*s == target) {
            return s;
        }

        s++;
    }

    return NULL;
}
```

If the target is found, the function returns a pointer to the matching character.

If the target is not found, it returns `NULL`.

Pointer subtraction can compute the offset from the beginning:

```c
found - word
```

This is valid only when both pointers point into the same array object, or one past it.

A returned pointer remains valid only as long as the object it points into remains alive.

## 20. Connection to NucleOS

Operating-system development relies heavily on pointers, arrays, buffers, and explicit traversal.

A kernel needs to walk arrays, tables, descriptors, and buffers.

A command parser needs to scan character input until a terminator.

A scheduler may walk task tables.

A memory manager may manipulate address ranges.

Drivers may read or write through pointers to memory-like regions.

Day 15 is directly connected to future NucleOS work because it introduces the mechanics behind:

* typed addresses
* indirect access
* array traversal
* string scanning
* output pointers
* null checks
* bounds
* lifetimes
* explicit ownership of writable storage

A future command parser in NucleOS will need to scan a buffer one character at a time, stop at terminators, compare command names, and return pointers or status codes safely.

## Practical takeaways

1. A pointer is a typed address value.
2. `&x` gives the address of `x`.
3. `*p` accesses the object pointed to by `p`.
4. Assigning to `*p` writes through the pointer.
5. C is always pass-by-value.
6. Passing a pointer means passing an address value by value.
7. Arrays are not pointer variables.
8. Array expressions often decay to pointers to their first element.
9. `a[i]` is equivalent to `*(a + i)`.
10. Pointer arithmetic advances by elements.
11. Array parameters lose size information.
12. Strings are null-terminated character arrays.
13. String literals must not be modified.
14. Destination buffers must have enough space.
15. One-past pointers may be compared but not dereferenced.

## Final observation

Day 15 showed that pointers are the point where C becomes serious.

They make memory traversal, string processing, array access, and output parameters possible, but they also make invalid addresses, missing bounds, and object lifetime unavoidable.

A pointer gives control, but that control is only safe when the pointed object is valid, the bounds are known, and the lifetime has not ended.
