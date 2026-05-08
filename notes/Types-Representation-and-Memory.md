# Notes — Types, Representation, and Memory

## Source

K&R Chapter 2, sections 2.1–2.3.

## What a C type actually is

A type in C does not describe meaning. It describes how the compiler
interprets memory and which optimizations become legal.

`int x` is four simultaneous instructions to the compiler:
reserve space aligned for an integer, interpret reads as two's complement
signed, enable integer arithmetic, and assume the value never overflows.
The last instruction is not enforced — it is assumed. If violated,
the compiler is permitted to emit anything.

A type is a contract between the programmer and the compiler.
The hardware knows nothing about types. It moves bits.

## Representation vs semantics

Three distinct layers exist. C conflates them. You should not.

**Physical representation** the actual bit pattern at a memory address.
`11000000` is eight bits. Nothing more.

**Type** — the compiler's rule for interpreting those bits.
As `unsigned char`: 192. As `signed char`: -64. Same bits. Different rules.

**Semantics** — what the programmer intends those bits to mean.
A character. A pixel value. A flag. An index.

The type constrains the interpretation. It does not capture the intent.
Two variables can share a type and mean entirely different things.
No mechanism in C enforces the gap between type and semantics.

## Integer promotions

C does not perform arithmetic on types smaller than `int`.
Before any arithmetic operation, `char` and `short` are promoted to `int`.
This happens silently. There is no syntax marking it.

```c
unsigned char a = 200;
unsigned char b = 100;
unsigned char c = a + b;
```

The addition occurs in `int`. `200 + 100 = 300` no overflow.
The truncation occurs on assignment: `300 % 256 = 44`.

The overflow does not happen where the operands live.
It happens where the result lands.

Programmers who assume arithmetic occurs at the width of the operands
will misread this program. The assumption is wrong. Promotions are
invisible and unconditional.

The same mechanism governs comparisons between `char` and integer
constants, bitwise operations on small types, and argument passing
to variadic functions. Promotions are not a corner case, they are
the default behavior of every expression involving small types.

## Signed and unsigned interpretation

Signedness is not a property of bits. It is an interpretation rule.

`11111111` as `unsigned char` is 255.
`11111111` as `signed char` is -128... no, it is -1 in two's complement.
The bits are identical. The values are not.

**Promotions and sign extension:**

When `signed char` is promoted to `int`, the sign bit is replicated
into the upper 24 bits. `11001000` becomes
`11111111 11111111 11111111 11001000` the value -56 preserved in 32 bits.

When `unsigned char` is promoted, the upper bits are filled with zero.
`11001000` becomes `00000000 00000000 00000000 11001000` the value 200.

Same byte. Two promotions. Two different integers passed to `printf`.

**Unsigned arithmetic is modulo 2ⁿ by definition.**
The standard guarantees it. The hardware implements it through truncation
to register width. There is no overflow, there is wrap, and wrap is defined.

**Signed overflow is undefined behavior.**
The compiler assumes it does not occur. If it does, the compiler may
eliminate the code that would have handled it. The hardware produces
a bit pattern. The optimizer may have already removed the instruction
that reads it.

**Signed and unsigned comparison:**

When a signed and unsigned value of the same width are compared,
the signed value is converted to unsigned. A negative signed value
becomes a large unsigned value. The comparison produces a result
opposite to mathematical expectation, silently.

## Constants and execution character sets

`'0'` is not the number 0. It is the integer value of the digit character
zero in the execution character set. In ASCII that value is 48.
The standard does not mandate ASCII.

What the standard does mandate: digit characters are contiguous and
ordered. `'0'` through `'9'` have consecutive values. `c - '0'`
produces the numeric value of a digit character regardless of encoding.
This is portability, not coincidence.

`'\0'` is integer zero. It is the null terminator. It is not related
to `'0'` except that both are integer constants interpreted as characters.

`sizeof('A')` is 4, not 1. Character constants in C have type `int`.
This surprises programmers who assume the type follows the notation.
The notation is for readability. The type is `int`.

Octal constants begin with `0`. `012` is ten, not twelve.
This produces silent bugs in code that pads integers with leading zeros
for visual alignment. The compiler does not warn.

## Arrays are not strings

`char buf[10]` expresses one thing: ten contiguous bytes in the current scope.

It does not express:

**Initialization** — the bytes contain whatever the hardware left at
those addresses. There is no default value.

**Valid length** — capacity is 10. The number of meaningful bytes
could be 0, 5, or 9. The type does not distinguish.

**Termination** — no null terminator is guaranteed. Functions that
assume one will read past the buffer until they find a zero byte elsewhere.

**Encoding** — the bytes could be ASCII, UTF-8, Latin-1, or arbitrary
binary data. The type expresses none of this.

**Ownership** — who is responsible for maintaining the null terminator
invariant? Who ensures no other pointer aliases this buffer?
The type does not say.

**Mutability** — `char buf[10]` is mutable. `const char buf[10]`
restricts access through that name. Neither guarantees what other
code can do through other pointers to the same memory.

**Lifetime** — the array lives until the scope ends. A pointer to it
can outlive the scope. The type does not track that promise.

The null terminator invariant — that a zero byte exists within the
valid region — is the programmer's contract. The compiler does not
enforce it. The runtime does not check it. Every string function
assumes it and produces undefined behavior when it is absent.

`char[10]` is storage. The string abstraction is a convention layered
on top of that storage by human agreement.

## Array decay and metadata loss

When an array is passed to a function, it decays to a pointer to
its first element. The type of the parameter becomes `char *`.

```c
void f(char s[])   /* equivalent to: void f(char *s) */
```

At the declaration site, the compiler knows the array has 10 elements.
That information exists in the type system. At the moment of decay,
it is discarded. What crosses the function boundary is an address.

`sizeof(s)` inside the function returns the size of a pointer,
4 or 8 bytes. The original size is gone. The compiler cannot recover
it because it was not encoded in what was passed.

This is not an unfortunate side effect. It is a deliberate trade-off:
arrays and pointers share arithmetic semantics, which simplifies the
compiler and the memory model. The cost is that every interface
involving arrays must carry the size as a separate argument or
rely on a sentinel value, and nothing enforces that this happens.

Decay is not a conversion. It is the loss of semantic metadata
at a boundary the type system cannot see across.

## Storage duration and ownership

Every object in C has a storage duration — the period during which
its memory is valid.

**Automatic** — local variables. Memory is valid within the enclosing
scope. When the scope ends, the memory may be reused. Pointers to
automatic variables that outlive their scope are dangling.

**Static** — global variables and string literals. Memory persists
for the entire program lifetime. String literals specifically have
static storage duration defined by the standard.

**Allocated** — `malloc`. Memory persists until `free` is called.
The type of the pointer carries no information about this obligation.
Ownership — the responsibility to call `free` exactly once — is
a human convention with no enforcement mechanism in the language.

## String literals vs stack arrays

```c
const char *p = "hello";
char        s[] = "hello";
```

The visible content is identical. Everything else differs.

`p` holds the address of a string literal in static storage.
That storage exists for the entire program lifetime.
Modifying it is undefined behavior, the standard does not guarantee
that it is physically read-only, but the compiler may optimize
assuming its content never changes. On most systems the OS maps
this region without write permission and a modification produces
a fault.

`s` is a stack array initialized by copying the literal's bytes.
The memory belongs to the current scope. It is writable.
When the scope ends, the memory is gone.

`sizeof(p)` is the size of a pointer. `sizeof(s)` is 6 —
five characters plus the null terminator — because `s` is the array,
not a reference to it.

Same bytes. Two storage classes. Two lifetimes. Two mutability rules.
The type notation does not make this visible.

## Undefined behavior and optimizer assumptions

UB is not "the program crashes." It is "the compiler is permitted
to assume this does not occur, and to optimize accordingly."

The compiler uses UB assumptions to eliminate code. If signed overflow
is UB, the compiler may assume a signed variable never overflows
and remove a check that would only matter if it did. The hardware
would have produced a bit pattern. The optimizer removed the instruction
that reads it.

```c
const int x = 42;
int *p = (int *)&x;
*p = 99;
printf("%d %d\n", x, *p);  /* may print: 42 99 */
```

The compiler substituted `42` for `x` at compile time, it had
a guarantee that `x` does not change. `*p` forced a memory read.
The physical memory contains `99`. The abstract value of `x` is `42`.
The model and the hardware disagree because UB allowed the compiler
to optimize past what the hardware would do.

The distinction between physical behavior and abstract machine behavior
is the difference between "it worked on my machine" and "it is correct."
UB produces code that is observably correct under one set of conditions
and silently wrong under another, a different optimization level,
a different compiler, a different call stack.

## Const is not immutability

`const` restricts access through a typed path. It does not make
an object immutable.

```c
const char *p   /* the object p points to cannot be modified through p */
char *const p   /* p itself cannot be modified — it always holds the same address */
const char *const p   /* neither p nor the object it points to can be modified through p */
```

In all three cases, another pointer without `const` can modify the
same memory. `const` is a compile-time restriction on one access path.
It is not a property of the memory.

The compiler may use `const` declarations to cache values in registers
and avoid repeated memory reads. If the underlying memory is modified
through another path, the cached value and the physical value diverge.
This is not a compiler bug, the programmer violated the contract
that `const` implies.

## Why C types are semantically incomplete

A type in C specifies:
- How many bytes to reserve
- How to interpret reads
- Which operations are syntactically valid
- What the compiler may assume

A type in C does not specify:
- What the value means
- Whether it has been initialized
- Who is responsible for it
- How long the pointer to it will remain valid
- Whether other code may modify it concurrently
- What invariants must hold for the value to be meaningful

Every gap between what the type specifies and what the program requires
is a contract the programmer must maintain manually.
The compiler cannot verify it. The runtime does not check it.
Violations produce undefined behavior, silent corruption, or
exploitable vulnerabilities, often all three, in sequence.

## Final observation

C types are storage and optimization contracts, not semantic guarantees.

The compiler uses the type to decide how to read memory and which
transformations are legal. The programmer uses the type to communicate
intent. These two uses are often aligned, but the alignment is
a convention, not an enforcement.

Every abstraction built on C types, strings, buffers, ownership
protocols — exists outside the type system. It exists in documentation,
in naming conventions, in discipline.

That is not a flaw in C. It is the design. C gives you the minimum
machinery to talk to the hardware and gets out of the way.
The cost of that minimalism is that correctness is entirely
the programmer's responsibility, with no structural support
from the language.

Understanding this is the prerequisite for everything else.
