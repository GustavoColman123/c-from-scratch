# Notes — Day 09 (External Variables and Scope)

## Personal deviations from K&R 1.10

The original K&R example keeps `max` as a local variable in `main`. Only `line` and `longest` are external. I chose to make `max` global as well for three reasons:

1. **Conceptual purity** — if the lesson is "external variables enable implicit communication", using a hybrid model (some globals, some locals) obscures the pattern.
2. **Consistency with the model** — `main` already reads `line` and `longest` globally. Reading `max` globally adds no new risk.
3. **Didactic clarity** — the pure `extern` version shows the full cost of globals: every piece of shared state is visible to every function.

K&R's choice to keep `max` local is reasonable for real code. My choice is better for understanding the mechanism.

## Other deliberate changes

| Aspect | K&R original | My version |
|--------|--------------|------------|
| `getline` name | Uses `getline` (now a POSIX function) | Renamed to `read_next_line` |
| Newline storage | Stores `'\n'` in the buffer | Discards `'\n'`, length excludes it |
| Empty line handling | Treated as line of length 0 | Ignored in result (loop condition `> 0`) |
| EOF distinction | Returns 0 for both empty line and EOF | Returns -1 for EOF with no input |
| Overflow behavior | Stops reading, leaves remainder in buffer | Discards remainder explicitly |
| `copy` after copy | No invalidation | No invalidation,`line` remains valid until next read |

These changes do not violate the lesson. They make the trade-offs of external variables *more* visible, not less.

## What the book does not say explicitly

K&R shows *that* external variables work. It does not emphasize:

- **No ownership** — any function can write any global. The program relies on programmer discipline.
- **No local reasoning** — you cannot understand `copy()` without knowing who writes `line[]`.
- **Testing difficulty** — globals persist across test cases. State leaks.
- **Thread safety** — is not provibed by the lenguage, shares globals requiere explicit synchronization.

The book assumes a single-file, single-thread, single-programmer world. That was reasonable in 1978. It is not reasonable today, but the mechanism remains in the language.

## Why this matters before Chapter 2

Chapter 1 ends with external variables. Chapter 2 begins with data types, operators, and expressions.

The bridge is this: **scope and visibility are the foundation for understanding type qualifiers (`static`, `extern`, `const`)**. Without knowing what external variables expose, you cannot understand why `static` restricts visibility or why `const` adds immutability.

Day 09 is the last time the book will explicitly teach global state as a *pattern*. From Chapter 2 onward, globals are a *tool* — you decide when to use them. The training wheels come off.

## Practical takeaways for later C

1. Prefer parameters over globals unless the state is truly global (configuration, logging, hardware registers).
2. If you must use globals, document *exactly* which functions write each variable.
3. Consider `static` at file scope to limit visibility to a single translation unit.
4. Accept that C will not protect you. The pointer is just a number. The number is treated as truth.

*Next: Chapter 2 — Data Types and Operators*
