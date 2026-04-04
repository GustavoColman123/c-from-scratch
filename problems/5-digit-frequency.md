# Problem 5 — Digit Frequency

## Mistake 1 — Using `char` for the counter array

**What I wrote**
```c
char digits[10];
```

**What I thought**
The array stores counts of characters. `char` seemed appropriate.

**What actually happens**
`char` overflows at 127 on signed systems.
A digit that appears 128 times wraps to a negative value.
The program compiles and runs silently with wrong output.

**Fix**
int digits[10];
Counters are not characters. The type should reflect what is being stored.


## Mistake 2 — Skipping the initialization loop

**What I wrote**
int digits[10];
// jumped straight to the while loop

**What I thought**
The array starts at zero automatically.

**What actually happens**
Local arrays in C are not zero-initialized.
Each position contains whatever bytes were in that memory before.
Counts start at garbage values and the output is wrong
with no warning from the compiler.

**Fix**
for (i = 0; i < 10; ++i)
    digits[i] = 0;
```
Global arrays zero-initialize automatically. Local arrays do not.
The distinction matters here.

## Mistake 3 — Indexing without the range guard

**What I wrote**
++digits[c - '0'];
```

**What I thought**
The subtraction produces the right index for any character.

**What actually happens**
`'a' - '0'` is `49`. `'/' - '0'` is `-1`.
Both are out-of-bounds accesses.
The program writes to memory it does not own.
No warning, no crash — just silent corruption.

**Fix**
if (c >= '0' && c <= '9')
    ++digits[c - '0'];

The guard and the subtraction are inseparable.
One without the other is incomplete.

## Mistake 4 — Declaring the array as `digits[9]`

**What I thought**
The highest digit is 9. Size 9 covers everything.

**What actually happens**
`digits[9]` has valid indices `0` through `8`.
Accessing `digits[9]` is out of bounds.
The digit `'9'` corrupts adjacent memory on every occurrence.

**Fix**
int digits[10];
Array size is element count, not maximum index.
The two differ by exactly one and the mistake is always silent.


## Mistake 5 — Assuming classification is optional

**What I thought**
Only digits need to be handled explicitly.
Other characters can be ignored.

**What actually happens**
Every input must be classified.
If a character is not accounted for,
it silently disappears from the program’s model.

This leads to incomplete or misleading results.

**Fix**
Define a complete classification:

- digit
- whitespace
- other

Every character must fall into exactly one category.
