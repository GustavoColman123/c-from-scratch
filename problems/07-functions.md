# Problem 7 — Functions: power(base, n)

## Mistake 1 — Prototype placed after main

**What I wrote**
int main(void)
{
    printf("%d\n", power(2, 5));
    return 0;
}

int power(int base, int n) { ... }

**What I thought**
The function is defined in the same file. The compiler will find it.

**What actually happens**
The compiler processes the file top to bottom.
When it sees `power(2, 5)`, it has no information about what `power` expects or returns.
It assumes a return type of `int` and does no type checking.
On some systems this works by accident. On others it produces wrong results or fails to link.

**Fix**
int power(int base, int n);  // prototype before main

int main(void) { ... }

int power(int base, int n) { ... }

The prototype gives the compiler the contract before the first call.

## Mistake 2 — Initializing p to 0 instead of 1

**What I wrote**
p = 0;
for (i = 1; i <= n; ++i)
    p = p * base;

**What I thought**
Zero feels like a neutral starting point for accumulation.

**What actually happens**
`0 * anything` is always `0`.
Every call to `power` returns `0` regardless of input.
The program compiles and runs without error.

**Fix**
p = 1;
Exponentiation is repeated multiplication, not repeated addition.
The identity element for multiplication is `1`, not `0`.

## Mistake 3 — Thinking modifications to parameters affect the caller

**What I wrote**
int power(int base, int n)
{
    base = base * 2;  // trying to double it for some reason
    ...
}

**What I thought**
Changing `base` inside the function would affect the variable passed from `main`.

**What actually happens**
C passes by value. `base` inside `power` is a copy.
The original variable in `main` is untouched.
The modification is real but invisible to the caller.

**No crash, no warning — just a misunderstanding of the model.**

**Fix**
Understand that parameters are local copies.
If the goal is to modify the original, that requires pointers.
If the goal is just local use, modifying parameters is valid — but intentional.

## Mistake 4 — Loop starting at i = 0 instead of i = 1

**What I wrote**
for (i = 0; i <= n; ++i)
    p = p * base;

**What I thought**
Loops start at 0. That is the default assumption from previous programs.

**What actually happens**
The loop runs `n + 1` times instead of `n`.
`power(2, 3)` returns `16` instead of `8`.
Off-by-one in the loop bound, not in the index.

**Fix**
for (i = 1; i <= n; ++i)
The loop counts multiplications, not indices.
One multiplication for each step from `1` to `n` inclusive — that is exactly `n` iterations.

## Mistake 5 — Using == to compare return type in a prototype mismatch

**What I wrote**
double power(int base, int n);  // prototype declares double

int power(int base, int n)      // definition returns int
{
    ...
    return p;
}

**What I thought**
The compiler would catch this and refuse to compile.

**What actually happens**
Some compilers warn. Some do not.
The caller reads the return value as `double` because the prototype said so.
The function returns an `int` stored in the wrong format.
The value received by the caller is garbage with no runtime error.

**Fix**
Keep the return type in the prototype and the definition identical.
The prototype is a contract. A contract that does not match the implementation is worse than no contract.
