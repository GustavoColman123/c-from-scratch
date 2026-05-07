# types representation and memory

## Description

This program classifies characters from stdin into digits, whitespace,
and other. It exists here as a vehicle for understanding what types
in C actually are, not abstractions, but contracts between the
programmer, the compiler, and the hardware.

## What a type actually is

A type is not a mathematical set. It is a set of promises about memory:
how many bytes to reserve, how to interpret those bytes, what operations
the compiler will emit, and what the compiler is allowed to assume.

`int x` does not mean "x is an integer." It means: reserve space for
an integer-sized object, interpret reads as two's complement signed,
and assume the value never overflows or the program has no defined behavior.

## Representation

Every type is a bit pattern with an interpretation rule.

`char` — one byte. The interpretation of the high bit is
implementation-defined. On some systems it is signed, on others
unsigned. This is not a footnote, it determines the behavior of
every comparison and promotion involving that byte.

`int` — typically 32 bits, but the standard only guarantees it is
at least 16 and no shorter than `short`. The actual size is a
compiler decision, not a language guarantee.

`float`, `double` — IEEE 754 format, implemented in a separate
hardware unit. The type determines which unit and which instruction set.

## Integer promotions

C does not perform arithmetic on types smaller than `int`.
Before any arithmetic operation, `char` and `short` are promoted to `int`.

```c
unsigned char a = 200;
unsigned char b = 100;
unsigned char c = a + b;  // arithmetic in 32 bits, truncation on assignment
```

The addition produces `300` as an `int`. The overflow does not occur
in 8-bit arithmetic, it occurs when 300 is truncated to 8 bits on
assignment to `c`. The result is `44`.

This surprises programmers who assume the type of the operands
determines the width of the operation.

## Overflow

Signed integer overflow is undefined behavior. Not "wraps around."
Not "produces a negative number." Undefined, the compiler may
eliminate code that assumes it can occur.

```c
int x = INT_MIN;
x = -x;  // UB — result is not representable
```

The hardware produces a bit pattern. The compiler may have already
eliminated the operation based on the assumption that UB never occurs.

Unsigned overflow is defined, it wraps modulo 2ⁿ. This is not
a safety guarantee. It means the truncation is predictable.
Predictable wrong values are still wrong values.

## ABI and calling conventions

`printf` is variadic. Arguments to variadic functions undergo
default argument promotions before being passed.

`char` — promoted to `int`. If signed, sign extension fills the
upper bits with the sign bit. If unsigned, zero extension fills
with zeros. Same bit pattern in the `char`, different `int` value.

```c
char c = 200;       // signed: -56. unsigned: 200.
printf("%d\n", c);  // signed machine: -56. unsigned machine: 200.
```

The ABI specifies what the callee receives. Promotions are the
mechanism by which values cross function boundaries.

## Aliasing and const

`const` is a type system restriction, not a memory guarantee.

```c
const int x = 42;
int *p = (int *)&x;
*p = 99;
printf("%d %d\n", x, *p);  // may print: 42 99
```

The compiler substitutes `42` for `x` at compile time, the variable
may never be read from memory. `*p` forces a memory read. Same address,
two different values printed, because the compiler and the memory
disagree about what is there.

Strict aliasing compounds this. A pointer of type `float *` pointing
to an `int` object is undefined behavior, the compiler assumes
pointers of incompatible types never alias, and optimizes away reads
that would only matter if they did.

## Storage duration and string literals

```c
char *p = "hello";  // p points to static read-only storage
char s[] = "hello"; // s is a mutable copy on the stack
```

The literal `"hello"` lives in the binary static storage duration,
entire program lifetime, mapped read-only by the OS.

`p` holds the address of that region. Writing through `p` is UB
even on hardware where no fault occurs the compiler assumes
string literals are never modified and may eliminate code that
would only matter if they were.

`s` is initialized from the literal but owns its bytes on the stack.
The copy is writable. The lifetime is the enclosing scope.

## Strings, invariants, and `char buf[10]`

`char buf[10]` expresses exactly one thing: ten contiguous bytes
in the current scope.

It does not express:
- Whether the bytes are initialized
- Whether a null terminator exists and where
- How many bytes contain valid data
- What encoding the bytes represent
- Whether other pointers alias the same memory
- Who is responsible for maintaining the null terminator invariant

Every string function in C assumes the null terminator invariant.
`strlen`, `strcpy`, `printf("%s")` all of them walk forward in
memory until they find a zero byte. If that byte does not exist
within the valid region, the behavior is undefined and the read
continues into adjacent memory.

The invariant is the programmer's contract. The type does not
enforce it. The compiler does not verify it. The runtime does not
check it.

## Ownership

C has no ownership system. Ownership is documentation.

```c
char *p = malloc(10);
```

`p` holds a valid address. Ten bytes are allocated. Nothing in the
type records that `p` is responsible for freeing them, that no other
pointer should outlive the allocation, or that the bytes require
initialization before use.

When an array is passed to a function, the caller owns the memory.
The callee receives an address and a length — if the length is passed
at all. Nothing prevents the callee from writing past the length.
The ownership contract exists in the programmer's head, not in the
type system.

## What `int x` actually means

It is not a mathematical statement. It is four simultaneous instructions:

1. Reserve space in the current stack frame, aligned for the target architecture.
2. Interpret reads from that space as a signed integer in two's complement.
3. Enable integer arithmetic and comparison operations on that space.
4. Assume the value is never read before it is written, and never overflows.

The last point is not enforced. It is assumed. If violated, the compiler
is permitted to emit code that does anything — including code that
appears correct in testing and fails in production under a different
optimization level.

The type system in C is a tool for the compiler. The programmer
benefits from it only insofar as they respect contracts the compiler
cannot verify.
