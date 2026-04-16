# Day 09 — External Variables and Scope

## What I built

Extended the longest line program from Day 08 to use external (global) variables instead of passing parameters between functions. This covers K&R Chapter 1.10.

The program reads input lines and prints the longest one, but now `line`, `longest`, and `max` are shared implicitly. Functions communicate through global state, not through arguments or return values.

## What changed from Day 08

Day 08 passed arrays as parameters. `get_line(buffer, size)` received the buffer address explicitly. `copy(to, from)` received both source and destination. Data flow was visible in function signatures.

Day 09 declares variables outside any function. `get_line(void)` writes directly to global `line[]`. `copy(void)` reads from global `line[]` and writes to global `longest[]`. No parameters. No return values for data transfer — only `len` coordinates the decision in `main`.

The program works the same from the outside. Inside, information travels invisibly.

## What I had to define before writing code

- Whether `len` means buffer capacity or stored length (chose: stored length, excludes `\n`)
- What happens to lines longer than `MAXLINE` (chose: truncate and discard remainder silently)
- Whether empty lines count as valid input (chose: ignored in final result)
- How to distinguish EOF from empty line (chose: `-1` return signals termination)
- Which variables are truly global (chose: `line`, `longest`, `max` — but `max` was local in original K&R)

## What clicked

**External variables break local reasoning.**

With parameters, you understand a function by looking at its signature and its body. With globals, you cannot. `copy()` has no parameters, but it depends on `line` being correctly null-terminated and `longest` having enough capacity. Those dependencies are nowhere in the prototype.

**Ownership does not exist in C.**

No variable has an owner. Any function can write to any global at any time. The program works because the programmer follows a discipline — not because the language enforces anything. The discipline is: only `read_next_line` writes to `line[]`, only `commit_to_longest` writes to `longest[]`. C trusts you. C does not check.

**The real problem of C is not insecurity. It is invisible invariants.**

The signature `void process(char *data, int len)` does not tell you whether `len` is the buffer size or the valid data length. That ambiguity causes real bugs. The language makes you document in comments what should be expressed in types.

## Key difference in thinking

Day 08: data flows through parameters. You trace the program by following arguments from caller to callee.

Day 09: data sits in globals. You trace the program by knowing which functions write which variables and in what order. The execution order becomes part of the contract.

## Mistakes / confusions

- Assumed `max` could stay local while `line` and `longest` were global. That is a hybrid model — neither pure `extern` nor pure parameter passing. Corrected by making `max` global.
- Used `c` after the read loop without guaranteeing it was initialized in all paths. The fix required restructuring to assign `c = getchar()` before the loop condition.
- Detected overflow by checking `i == MAXLINE - 1` alone. This fails when a line is exactly `MAXLINE - 1` characters followed by `\n`. Fixed by tracking exit reason explicitly.
- Initially returned `0` for both empty line and EOF. Could not distinguish them. Changed to return `-1` for EOF with no characters, `0` for valid empty line, `>0` for stored length.

## What I would change in C (minimally)

Without breaking C's philosophy:

1. **Array decay only with explicit `&` operator** — no silent pointer conversion. You know exactly when you lose size information.

2. **Function parameters of type `T a[N]` actually mean N elements** — compiler warns on mismatched sizes. Use `T *a` when you want flexibility.

These changes do not add runtime checks. They make existing invariants visible. C remains fast and low-level, but the programmer is no longer blind.

## Time

90 min (including error analysis and redesign)
