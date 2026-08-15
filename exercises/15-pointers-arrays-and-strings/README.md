# Pointers, Arrays, and Strings

## Overview

This exercise covers the foundational pointer, array, and string mechanics from K&R Chapter 5.

It focuses on:

* 5.1 Pointers and Addresses
* 5.2 Pointers and Function Arguments
* 5.3 Pointers and Arrays
* 5.4 Address Arithmetic
* 5.5 Character Pointers and Functions

The goal is to understand how C uses typed address values to access objects indirectly, traverse arrays, scan strings, and allow functions to modify caller-owned data safely.

## Concepts Covered

* Address-of operator `&`
* Indirection operator `*`
* Pointer declarations and pointer types
* Pass-by-value
* Passing address values by value
* Wrong swap vs pointer-based swap
* Output pointers
* `NULL` checks before dereferencing
* Array-to-pointer conversion
* `a[i]` and `*(a + i)`
* Pointer arithmetic by elements
* One-past pointers
* Subarray views
* Character arrays vs string literals
* Null-terminated strings
* String traversal, copy, comparison, and search

## Program Structure

The exercise is implemented in one file:

```text
main.c
```

The program is divided into nine sections:

1. Address and indirection
2. Swap and pass-by-address
3. Output pointer
4. Arrays and pointer arithmetic
5. Subarray view
6. Character arrays and string literals
7. String traversal
8. String copy and compare
9. Pointer search

## Example Build

Compile the program:

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic main.c -o demo
```

Run it:

```bash
./demo
```

Remove the generated binary before committing:

```bash
rm demo
```

## Key Observations

* A pointer is a typed address value.
* `&x` produces the address of `x`.
* `*p` accesses the object pointed to by `p`.
* Assigning through `*p` modifies the pointed object.
* C always passes arguments by value.
* Passing a pointer means passing an address value by value.
* Arrays are not pointer variables.
* Array expressions often decay to pointers to their first element.
* `a[i]` is equivalent to `*(a + i)`.
* Pointer arithmetic moves by elements, not raw bytes.
* Array parameters lose size information.
* One-past pointers may be formed and compared, but not dereferenced.
* C strings are null-terminated character arrays.
* String literals must not be modified.
* Destination buffers must have enough writable space before copying.

## Connection to Systems Programming

This exercise is directly connected to systems programming.

Operating-system code constantly works with arrays, buffers, strings, addresses, tables, and caller-owned output locations.

A command parser scans character buffers.

A scheduler walks task tables.

A memory manager handles address ranges.

A driver may access memory-like device regions through pointers.

The mechanics in this exercise are small, but they are the same mechanics that future NucleOS code will depend on:

* typed addresses
* indirect access
* explicit traversal
* null checks
* bounds
* string termination
* writable buffers
* object lifetime

## Lessons Learned

Day 15 showed that pointers are where C becomes serious.

Pointers make memory traversal, array access, string processing, output parameters, and low-level interfaces possible.

They also make invalid addresses, missing bounds, null pointer checks, object lifetime, and writable storage unavoidable.

The main lesson is that C gives the programmer direct control over how objects are reached.

That control is powerful, but it must be paired with explicit checks, clear bounds, and precise ownership.

## Source

Kernighan & Ritchie, Chapter 5:

* 5.1 Pointers and Addresses
* 5.2 Pointers and Function Arguments
* 5.3 Pointers and Arrays
* 5.4 Address Arithmetic
* 5.5 Character Pointers and Functions
