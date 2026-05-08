# Day 10 — Types, Representation, and Memory

## What I expected before studying Chapter 2

I expected a chapter about syntax. Names, types, operators, the vocabulary of the language. Reference material.

I did not expect it to be about the relationship between the programmer,
the compiler, and the hardware. That is what it turned out to be.

## What changed in my mental model

Before this chapter, I read `int x` as a declaration.
A statement that x exists and holds an integer.

Now I read it as a contract with four terms: reserve aligned storage,
interpret reads as two's complement signed, enable integer operations,
and assume no overflow ever occurs. The last term is not enforced.
It is assumed. The compiler optimizes based on that assumption.
Violating it does not produce an error, it produces a program
that the compiler transformed based on a false premise.

That shift, from "declaration" to "contract with optimizer consequences"
changed how I read every line of C I had written before.

## Most important realization

The type does not describe the value. It describes how bits are interpreted
and what the compiler is permitted to do.

`11001000` is not -56 or 200. It is eight bits. The type decides which.
And the type can change at a boundary, a promotion, a cast, a function call,
silently, without any visible syntax.

The moment that became concrete was the `char c = 200` example.
Same bit pattern. Two different integers in `printf`. Not because
the hardware did something different, because the promotion
applied a different rule to the same bits.

I had been thinking of types as labels on values.
They are rules for interpreting storage. Those are not the same thing.

## Concepts that looked simple but were not

**`const`** I assumed it meant immutable. It means: the compiler will
reject modifications through this name, and will optimize assuming
the value does not change through any path. The second half is what
makes violating it dangerous. The cast silences the type checker.
It does not silence the optimizer.

**Character constants** — `'0'` has type `int`. Not `char`. The notation
suggests one thing; the type is another. `sizeof('A')` is 4.
I had been reading character literals as if they were values in `char`.
They are values in `int` that happen to fit in `char`.

**`"\0"`** I understood it as a null terminator. It is a string literal
whose explicit content is one null character, followed by the implicit
terminator. Two zero bytes in memory. The content and the terminator
are distinct conceptually even when they are identical physically.

## Where the language became dangerous

Not at pointers. Not at manual memory management.

At invisible assumptions.

Integer promotions happen before every arithmetic operation on small types
and leave no trace in the source. The overflow that seems to happen
at 8 bits actually happens at assignment. The check that seems to
detect signed overflow may have been removed by the optimizer.
The `const` object that seems immutable can be modified through
a cast with UB the compiler cannot see.

Every one of these is silent. None of them produce compile errors.
Several produce no runtime error either, just wrong behavior
in specific conditions that may not appear in testing.

The danger in C is not that things visibly break.
It is that things invisibly work, until they do not.

## Connections to previous chapters

The null terminator invariant was already present in Chapter 1.
`getline` depended on it. `copy` produced it. I treated it as
a mechanical detail — write `'\0'` at the end.

Now I understand what it actually is: a contract outside the type system
that every string function assumes and that nothing enforces.
`char buf[MAXLINE]` in the longest-line program was not a string.
It was storage plus a manually maintained invariant.
The program worked because the invariant was respected.
If it had been broken on any path, `printf("%s")` would have read
past the buffer with no indication.

The digit frequency program used `digits[c - '0']`. I understood
`c - '0'` as a mapping. Now I understand why it is portable:
the standard guarantees digit characters are contiguous and ordered.
The subtraction cancels the encoding offset. It works because
the type rules — specifically, the execution character set guarantee,
make it work, not because of ASCII.

## Connections to systems programming

Every concept in this chapter is a prerequisite for anything below
the application layer.

Buffer sizes without size metadata, every network protocol parser
that has ever had a CVE.

Signed/unsigned mismatch in comparisons — the class of vulnerabilities
where an attacker passes `-1` as a `size_t` and triggers an allocation
of `SIZE_MAX` bytes.

UB in arithmetic, the optimizations that removed security checks
from production kernels because the compiler proved the check guarded
against something the standard said could not happen.

`const` and aliasing — the reason `restrict` exists in C99,
and the reason the Linux kernel has its own aliasing annotations.

These are not advanced topics. They are Chapter 2 applied at scale.

## Questions that appeared while studying

If the compiler can substitute a `const` value at compile time,
does the variable need to exist in memory at all?
And if it does not, what does taking its address mean?

Why does the standard permit `char` signedness to be implementation-defined
but mandate that digit characters are contiguous?
One is a convenience for implementors. The other is a guarantee
for programmers. The asymmetry seems deliberate.

Integer promotions convert small types to `int` before arithmetic.
What happens on a machine where `int` is 16 bits and the result
overflows `int`? The promotion happened. The overflow is still UB.
Is the programmer expected to track the target's `INT_MAX`
in every arithmetic expression?

## What still feels unresolved

The abstract machine model. K&R does not name it, but it is the
real subject of this chapter. The standard defines behavior in terms
of what an abstract machine would do not what the hardware does.
The compiler's job is to produce hardware behavior that matches
the abstract machine behavior on all defined inputs.

On undefined inputs, the compiler may produce anything, because
the abstract machine's behavior is unspecified and the compiler
has no obligation to match it.

I understand the mechanism. I do not yet have intuition for where
the abstract machine and the hardware reliably diverge in practice.
That probably requires reading more optimized assembly output.

## Final reflection

Before this chapter, I thought the dangerous parts of C were the
explicit ones, pointer arithmetic, manual allocation, unchecked array access.
Those are visible. They look dangerous. Programmers look at them carefully.

The assumptions are invisible. Promotions leave no syntax.
Optimizer reasoning leaves no source-level trace. `const` looks like
a guarantee but is a restriction on one access path. `char[10]`
looks like a string but is storage without invariants.

The most dangerous thing in C is not memory corruption.
It is invisible assumptions, made by the type system, the promotions,
the optimizer, and the calling convention, that silently determine
whether the program does what the source appears to say.

Chapter 2 is where C reveals that the source code is not the program.
The source code is a description of intent, filtered through
a contract with the compiler, executed on hardware that knows
nothing about types.

That gap between intent and execution is where bugs live.

## Time
120 min
