# Problems — Types, Representation, and Memory

## Problem 1 — Assuming `char` has one fixed signedness

**What I thought**

`char` is a character type. Characters are positive values.
Therefore `char` behaves like `unsigned char`.

**What actually happens**

The signedness of plain `char` is implementation-defined.
The standard permits compilers to treat it as either signed or unsigned.
On x86 Linux with GCC, `char` is signed. On ARM embedded targets,
it is often unsigned. Both are conforming implementations.

```c
char c = 200;
printf("%d\n", c);  /* signed char: -56. unsigned char: 200. */
```

When `c` is promoted to `int` for `printf`, sign extension fills
the upper 24 bits with the sign bit. A signed `char` containing
`11001000` becomes the `int` `11111111 11111111 11111111 11001000` — value -56.
An unsigned `char` containing the same bits becomes `00000000 00000000 00000000 11001000` — value 200.

Same source. Same bit pattern. Two different programs depending on target.

**Why this matters**

Code that uses `char` to store values above 127, pixel data, encoded bytes,
extended ASCII, produces different results on different platforms.
The bug is silent. The compiler does not warn. The behavior is
implementation-defined, not undefined, so it is consistent on any
single platform and only fails when ported.

**Correct model**

Use `unsigned char` when the value is a byte or pixel.
Use `signed char` when the value is a small signed integer.
Use plain `char` only for actual text characters, and never compare
it against values outside `[0, 127]` without an explicit cast.

## Problem 2 — Assuming arithmetic happens at the width of the operands

**What I thought**

`unsigned char a = 200; unsigned char b = 100;` — both are 8-bit types.
Their sum is computed in 8 bits. Overflow occurs at 256.

**What actually happens**

Before any arithmetic, both operands are promoted to `int`.
The addition `200 + 100 = 300` occurs in 32-bit signed integer arithmetic.
There is no overflow. The result `300` is a valid `int`.

The truncation occurs on assignment to `unsigned char`:
`300 % 256 = 44`. The value `44` is stored in `c`.

The overflow is not in the arithmetic. It is in the conversion
back to the smaller type. These are different operations
at different points in the expression.

**Why this matters**

A programmer who believes the overflow occurs at 8 bits will expect
the result to be `300 - 256 = 44` — which happens to be correct,
but for the wrong reason. When the expression is more complex:

```c
unsigned char result = (a * b) / 100;
```

the intermediate `a * b = 20000` fits in `int` and the division
produces `200`. If the programmer assumed 8-bit arithmetic throughout,
they would predict `(200 * 100) % 256 = 32` divided by `100`, wrong.
The actual result is correct because promotions keep intermediate
values in `int`.

Relying on the wrong model produces wrong predictions for complex expressions.

**Correct model**

Arithmetic on types smaller than `int` always occurs in `int`.
Truncation back to the small type occurs only on assignment.
Track promotions and truncations as separate events.

## Problem 3 — Assuming signed overflow wraps like hardware

**What I thought**

`INT_MAX + 1` produces `INT_MIN`. The bits wrap around in two's complement.
The hardware does this. The program does this.

**What actually happens**

The hardware does wrap. The compiler does not guarantee it.

Signed integer overflow is undefined behavior in the C standard.
The compiler is permitted to assume it never occurs. If it assumes
overflow cannot happen, it may eliminate code whose only purpose
is to handle the overflow case:

```c
if (x + 1 < x)   /* detect overflow */
    handle();
```

The compiler sees: if `x` is `int`, `x + 1 < x` is only possible
if overflow occurred. Overflow is UB. Therefore this condition
is never true. The compiler removes the branch. `handle()` is
never called, even when `x` is `INT_MAX`.

The bit pattern that the hardware would have produced never reaches
a register that any instruction reads, because the optimizer removed
the instruction before emitting code.

**Why this matters**

The abstract machine model and the physical hardware model diverge
at UB. "It works on my machine" means the optimizer did not
exploit the UB assumption, yet. Under a different optimization
level, a different compiler version, or a different call context,
the same source produces different behavior.

**Correct model**

Signed overflow in C is not wrap-around. It is a promise to the
compiler that the event never occurs. The optimizer uses that
promise to transform the program. Breaking the promise produces
a program that the compiler transformed based on a false assumption.
The result is not incorrect arithmetic, it is arbitrary behavior.

## Problem 4 — Confusing `0`, `'0'`, `'\0'`, and `"\0"`

**What I thought**

These are different notations for roughly the same thing, zero,
or an empty/null value.

**What actually happens**

They are four distinct objects with different types, representations,
storage, and semantics.

`0` — type `int`, value zero. Four bytes: `00 00 00 00`.
No storage duration as a literal. Used as a number, a null pointer
constant, and a false value in boolean context.

`'0'` — type `int` in C, value 48 in ASCII-compatible systems.
It is the integer value of the digit character zero in the
execution character set. It is not zero. Comparing `c == '\0'`
and `c == '0'` tests for entirely different conditions.

`'\0'` — type `int`, value zero. Identical to `0` in value and type.
The notation communicates intent: this is a null terminator,
not an integer zero. The compiler sees the same bits.

`"\0"` — type `char[]`, two bytes: one explicit `'\0'` from the
content, one implicit `'\0'` as the terminator. Static storage
duration. It is a string literal whose visible content is empty
but whose explicit character is a null byte.

**Why this matters**

```c
char buf[10];
buf[0] = '0';   /* stores 48 — the digit character */
buf[0] = '\0';  /* stores 0  — terminates the string */
```

Storing `'0'` where `'\0'` is required leaves the string unterminated.
`strlen` and `printf("%s")` will read past `buf[0]` into whatever
memory follows. The bug compiles without warning and the error
appears far from its source.

**Correct model**

`'0'` is a digit. `'\0'` is a terminator. They share no relationship
beyond both being integer constants. Never substitute one for the other.

## Problem 5 — Assuming arrays carry their size into functions

**What I thought**

`sizeof(s)` reports the size of the array `s`.
Inside a function that receives `s`, `sizeof(s)` still reports
the size of the array.

**What actually happens**

At the declaration site — `char s[100]` — the compiler knows the
size. `sizeof(s)` is 100.

When `s` is passed to a function, array decay occurs. The array
converts to a pointer to its first element. The type of the parameter
is `char *` regardless of whether the declaration says `char s[]`
or `char *s`. They are identical to the compiler.

`sizeof(s)` inside the function returns `sizeof(char *)` — 4 or 8 bytes.
The original size was not encoded in the pointer. It cannot be recovered.

**Why this matters**

```c
void clear(char s[])
{
    memset(s, 0, sizeof(s));  /* clears 8 bytes, not the array */
}
```

This appears to clear the array. It clears 8 bytes, the size of
a pointer on a 64-bit system. The rest of the array is untouched.
The compiler does not warn. The bug is invisible at the call site.

**Correct model**

Arrays decay to pointers at function boundaries. Decay is metadata loss,
the size that existed in the type system is discarded and cannot be
recovered inside the function. Every interface that operates on arrays
must pass the size as an explicit separate argument.

## Problem 6 — Assuming `char buf[10]` is a string

**What I thought**

`char buf[10]` declares a string that can hold up to 10 characters.

**What actually happens**

`char buf[10]` declares ten contiguous bytes of automatic storage.
Nothing more. Every property required for it to function as a string
is absent from the type and must be established and maintained manually.

The invariants that string functions require, and that `char[10]` does
not provide:

**Initialization:** the bytes contain whatever the hardware left at
those addresses. Reading them before writing is UB.

**Termination:** no null terminator exists until the programmer writes one.
`strlen`, `strcpy`, and `printf("%s")` assume it exists and walk forward
in memory until they find a zero byte. If none exists within the
array, they read adjacent memory, stack variables, return addresses,
heap metadata.

**Valid length:** capacity is 10. Meaningful content could occupy 0 to 9 bytes.
The type does not distinguish between them.

**Encoding:** the bytes could be ASCII, UTF-8, or binary data.
No function can detect the encoding from the type.

**Why this matters**

The null terminator invariant is the programmer's contract with every
string function in the C standard library. The type does not enforce it.
The compiler does not verify it. Violations produce reads into
unowned memory, a source of information leaks and memory corruption.

**Correct model**

`char[10]` is storage. A string is storage plus the null terminator
invariant plus a known valid length. The type provides the first.
The programmer is responsible for the rest, on every code path,
including error paths and early returns.

## Problem 7 — Thinking `const` means physical immutability

**What I thought**

`const int x = 42` means `x` cannot be changed.
Any attempt to change it will fail or be caught.

**What actually happens**

`const` restricts access through one typed path. It does not make
the underlying memory physically unwritable.

```c
const int x = 42;
int *p = (int *)&x;
*p = 99;
printf("%d %d\n", x, *p);  /* may print: 42 99 */
```

The cast discards the `const` qualifier for the type checker.
The compiler had already used the `const` declaration to substitute
`42` for every read of `x` at compile time, no runtime read of
`x`'s address is emitted. `*p` forces a memory read, which returns
`99` from the physical address. `x` returns the compile-time constant.

Same address. Two different values printed. The type system and the
physical memory disagree because the compiler optimized based on
a contract the programmer then violated.

**Why this matters**

Code that casts away `const` and modifies the object produces UB.
The compiler's optimization assumptions about `const` objects are
no longer valid, but the compiler cannot detect the violation.
The result is a program that behaves differently under different
optimization levels, correct at `-O0`, wrong at `-O2`.

**Correct model**

`const` restricts accesses through a typed path. It is a compile-time
restriction enforced by the type checker, not a runtime property
of the memory. It enables optimizer assumptions. Violating those
assumptions through casts produces UB, not a compile error.

## Problem 8 — Thinking string literals are mutable because `char *p` compiles

**What I thought**

```c
char *p = "hello";
p[0] = 'H';
```

This compiles. Therefore it is valid. The program should print "Hello".

**What actually happens**

The compiler places `"hello"` in the binary's read-only data section.
`p` holds the address of that region. On modern operating systems,
the OS maps that section without write permission.

`p[0] = 'H'` attempts to write to that address. On most systems:
segmentation fault. On systems without memory protection: silent
corruption of a region shared across all uses of the literal,
potentially modifying other string literals that the compiler
merged into the same storage.

The assignment compiles because the historical type of a string
literal in C is `char *`, not `const char *`. This is a known
defect in the type system retained for backward compatibility.
The compiler may warn. It is not required to.

**Why this matters**

The compiler may use the same storage for identical literals:

```c
char *p = "hello";
char *q = "hello";
/* p and q may be the same address */
```

If modifying through `p` were valid, it would silently modify `q`.
The optimizer assumes literals never change. Code that relies on
that assumption, caching a read, emitting the value as an immediate, produces wrong results if any pointer modifies the literal.

**Correct model**

Declare pointers to string literals as `const char *`.
Treat modification of a string literal as UB regardless of whether
the compiler warns or the hardware faults. The correct form:

```c
const char *p = "hello";  /* type-correct, modification rejected at compile time */
char s[] = "hello";       /* mutable copy on the stack */
```

---

## Problem 9 — Thinking `malloc(10)` provides a buffer with meaning

**What I thought**

```c
char *p = malloc(10);
```

`p` now points to a 10-byte string buffer, ready to use.

**What actually happens**

`malloc` returns a pointer to 10 uninitialized bytes on the heap.
Three things are true. Three things are absent.

**True:** the address is valid. Reads and writes within 10 bytes
are memory-safe. The allocator has recorded that this region belongs
to the program until `free` is called.

**Absent:**

*Initialization* the bytes contain allocator metadata, fragments
of previous allocations, or arbitrary values. Reading them before
writing is UB.

*Termination* no null terminator exists. Passing `p` to any
string function before writing a null byte produces reads past
the allocation boundary.

*Ownership record* nothing in the type records that `p` is
responsible for calling `free`. Nothing prevents a second `free`.
Nothing prevents the pointer from outliving the allocation.
These obligations exist in the programmer's intent only.

**Why this matters**

```c
char *p = malloc(10);
printf("%s\n", p);      /* UB: no null terminator */
strcpy(p, "hello world");  /* UB: writes 12 bytes into 10-byte region */
free(p);
free(p);                /* UB: double free */
```

Each line compiles. Each line is undefined behavior.
The type `char *` carries none of the information needed to prevent
any of these mistakes.

**Correct model**

`malloc` returns ownership of uninitialized storage.
Ownership means the obligation to initialize before reading,
to stay within bounds, and to call `free` exactly once.
None of these obligations are expressed in the type.
All of them are the programmer's responsibility.
The type system provides no assistance and no enforcement.
