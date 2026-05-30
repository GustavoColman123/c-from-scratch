# Types, Representation, and Memory

## Description

This program explores how C types, constants, and arrays expose representation-level behavior.

It does not implement a K&R exercise directly. Instead, it is a small inspection program built after studying K&R Chapter 2 sections 2.1–2.3:

- variable names
- data types and sizes
- constants
- character constants
- integer promotions
- arrays versus pointers
- string literals versus mutable arrays

The goal is not to produce useful output, but to make hidden rules visible.

---

## Build and Run

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic main.c -o type_inspector
./type_inspector
```

---

## What the Program Demonstrates

### 1. Constants are not interchangeable

The program compares:

- `0`
- `'0'`
- `'\0'`
- `"0"`
- `"\0"`

These look similar but have different types, representations, and meanings.

`'0'` is a character constant with type `int` in C.  
`'\0'` is also an `int`, but its value is zero.  
`"0"` and `"\0"` are string literals, not single characters.

---

### 2. Type sizes are implementation-dependent

The program prints `sizeof` results for basic C types.

C guarantees relationships and minimum sizes, not exact universal sizes. For example:

- `sizeof(char)` is always `1`
- `int` is at least 16 bits
- `short` is no longer than `int`
- `int` is no longer than `long`

Modern systems commonly use 8-bit bytes and 32-bit `int`, but portable C code should not blindly depend on that.

---

### 3. Limits reveal representation boundaries

The program prints constants from `<limits.h>` such as:

- `CHAR_MIN`
- `CHAR_MAX`
- `SCHAR_MIN`
- `SCHAR_MAX`
- `UCHAR_MAX`
- `INT_MIN`
- `INT_MAX`
- `UINT_MAX`

This shows that plain `char` is not guaranteed to be signed or unsigned. Its signedness is implementation-defined.

---

### 4. Integer promotions happen before arithmetic

This expression:

```c
unsigned char a = 200;
unsigned char b = 100;
unsigned char c = a + b;
```

does not perform 8-bit arithmetic.

Both operands are promoted before addition. The arithmetic happens as `int`, producing `300`. Only when the result is assigned back to `unsigned char` is it converted modulo 256, producing `44`.

The important distinction:

- arithmetic happens after promotion
- narrowing happens on assignment

---

### 5. Arrays and pointers are not the same object

The program compares:

```c
const char *p = "hello";
char s[] = "hello";
```

`p` holds the address of a string literal. The literal has static storage duration, and modifying it is undefined behavior.

`s` is an array initialized with the same characters. It owns its storage and can be modified within its lifetime.

Same visible text. Different memory. Different rules.

---

## Key Insight

C types do not fully describe meaning.

A type tells the compiler:

- how much storage is needed
- how to interpret stored bits
- which operations are allowed
- which conversions may occur
- which assumptions can be used for optimization

But a type often does **not** express:

- ownership
- valid length
- initialization state
- null termination
- encoding
- aliasing intent
- lifetime safety

Those invariants must be maintained by the programmer.

---

## Important Distinction

This program intentionally avoids undefined behavior.

It discusses dangerous cases such as writing to string literals or signed integer overflow, but does not execute them. The purpose is to observe defined behavior and use it to reason about the rules behind the language.

---

## Source

Inspired by:

- K&R Chapter 2.1 — Variable Names
- K&R Chapter 2.2 — Data Types and Sizes
- K&R Chapter 2.3 — Constants
