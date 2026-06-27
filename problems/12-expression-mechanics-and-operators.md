# Problems — Day 12 (Expression Mechanics and Operators)

## Problem 1 — Forgetting parentheses around assignment in `getchar`

**What I wrote**

```c
while (c = getchar() != EOF) {
    /* process c */
}
```

**What I thought**

I thought the assignment would happen first. My mental model was:

```c
while ((c = getchar()) != EOF)
```

I expected `c` to receive each character from the input stream, and then be compared against `EOF`.

**What actually happens**

The relational operator `!=` has higher precedence than assignment `=`.

So the expression is parsed as:

```c
while (c = (getchar() != EOF))
```

This means `getchar() != EOF` is evaluated first. That comparison produces either `1` or `0`. Then that boolean-like integer result is assigned to `c`.

As a result, `c` does not store the character that was read. It stores only `1` while input continues, and `0` at EOF.

**Fix**

Explicitly group the assignment before the comparison:

```c
while ((c = getchar()) != EOF) {
    /* process c */
}
```

The assignment stores the character in `c`, and the assignment expression also produces the assigned value so it can be compared against `EOF`.

## Problem 2 — Treating plain `char` as a safe byte container

**What I wrote**

```c
char byte = 150;

if (byte > 0) {
    printf("positive\n");
} else {
    printf("negative\n");
}
```

**What I thought**

I thought `char` was just a small integer container that could safely hold byte-sized numeric values.

Since `150` is positive, I expected the condition `byte > 0` to be true.

**What actually happens**

Plain `char` may be signed or unsigned depending on the implementation.

On my system, `char` behaves as signed by default. If `char` uses 8 bits, the signed range is typically `-128` to `127`. The value `150` does not fit in that range.

When an integer value that cannot be represented is converted to a signed character type, the result is implementation-defined. On my system, the stored bit pattern was interpreted as a negative value, so the condition failed.

**Fix**

When using a byte as raw numeric storage or for bit manipulation, make the signedness explicit:

```c
unsigned char byte = 150;
```

If the value is meant to represent a character, use `char`.

If the value is meant to represent raw byte data, flags, or bit patterns, use `unsigned char` or a fixed-width unsigned type.

## Problem 3 — Treating `i++` versus `++i` as a performance problem

**What I wrote**

```c
for (int i = 0; i < 100000000; i++) {
    /* work */
}
```

Then I tried:

```c
for (int i = 0; i < 100000000; ++i) {
    /* work */
}
```

**What I thought**

I expected one version to be measurably faster than the other.

Since `++i` produces the incremented value directly and `i++` produces the old value, I assumed the machine would need to do extra work for `i++`.

**What actually happens**

When the produced value of the increment expression is not used, both forms have the same observable effect for an ordinary integer variable:

```c
i++;
++i;
```

Both increment `i` by one.

A modern optimizing compiler can see that the produced value is discarded, so it can generate the same machine code for both forms.

The important difference between prefix and postfix increment is semantic, not usually performance-related in this context:

```c
x = i++; /* use old value, then increment */
x = ++i; /* increment, then use new value */
```

**Fix**

Choose `i++` or `++i` based on the value you need, not based on imagined performance.

For standalone increments or the increment clause of a simple `for` loop, either form is usually fine for scalar integer variables.

## Problem 4 — Forgetting that the conditional operator has a common type

**What I wrote**

```c
int ajuste = 10;
double factor = 0.5;

double resultado = (ajuste > 5) ? ajuste : factor;
```

**What I thought**

I thought that if `ajuste > 5` was true, the conditional expression would simply return the integer `ajuste` as an integer.

My mental model was that each branch preserved its original type independently.

**What actually happens**

The conditional operator is an expression, and the expression has a type.

When the second and third operands have arithmetic types, C applies its usual conversion rules to determine a common type for the conditional expression.

Here, one branch is `int` and the other is `double`, so the conditional expression has type `double`.

That means even when the selected value comes from `ajuste`, the produced value is converted to `double`.

The expression produces `10.0`, not an integer `10`.

**Fix**

Keep the branches type-compatible with the kind of value the expression is supposed to produce.

If the result is meant to stay integral, use integral operands:

```c
int ajuste = 10;
int fallback = 0;

int resultado = (ajuste > 5) ? ajuste : fallback;
```

If the result is meant to be floating-point, accept the conversion deliberately:

```c
double resultado = (ajuste > 5) ? (double) ajuste : factor;
```

Do not feed a floating-point conditional result into bitwise logic. Bitwise operators require integer operands.

## Problem 5 — Expecting right shift on a signed negative value to fill with zeros

**What I wrote**

```c
int registro = -8;
registro = registro >> 2;
```

**What I thought**

I expected the right shift to move all bits to the right and fill the new left-side bits with zeros.

My mental model was a logical shift.

**What actually happens**

Right shift behaves cleanly for unsigned integers: the vacated left-side bits are filled with zeros.

For signed negative integers, the result of right shift is implementation-defined. Many systems perform an arithmetic shift, which fills the new left-side bits with copies of the sign bit.

That preserves the negative sign instead of injecting zeros.

So if the goal is raw bit manipulation, using a signed negative value makes the result depend on implementation behavior.

**Fix**

Use unsigned types for bit manipulation:

```c
unsigned int registro = 0xfffffff8u;
registro = registro >> 2;
```

The rule is:

```text
Use signed integers for arithmetic values.
Use unsigned integers for bit patterns, masks, flags, and shifts.
```

## Hidden invariants

* Assignment has lower precedence than relational operators.
* Assignment expressions produce values.
* Plain `char` signedness is implementation-defined.
* Prefix and postfix increment differ when their produced value is used.
* Compound and conditional expressions have type rules, not just runtime behavior.
* Bitwise operators require integer operands.
* Right shift of signed negative values is implementation-defined.
* Unsigned integers are the correct default for raw bit manipulation.

## Summary of corrections

| Problem              | Correct model                                                                          |
| -------------------- | -------------------------------------------------------------------------------------- |
| `getchar` assignment | Parenthesize assignment before comparing against `EOF`                                 |
| Plain `char`         | Not a portable raw byte container unless signedness is explicit                        |
| `i++` vs `++i`       | Semantic difference, not usually a performance difference for discarded scalar results |
| Conditional operator | Produces one value with one common type                                                |
| Signed right shift   | Use `unsigned` for predictable bit manipulation                                        |

## Final observation

The dangerous part of these sections is that the code often looks compact and elegant while hiding type conversions, side effects, and implementation-dependent behavior.

Day 12 showed that C expressions are active structures. They do not merely compute values. They may assign, convert, shift, mask, toggle, select, and modify state.

This matters for systems programming because NucleOS will eventually depend on exactly these mechanisms: flags, masks, byte-level data, status bits, and predictable manipulation of machine-sized values
