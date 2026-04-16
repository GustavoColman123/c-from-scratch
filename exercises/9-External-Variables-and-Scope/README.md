# External Variables and Scope

## Description

Reads input lines and prints the longest one. Uses external (global) variables to store the current and longest lines, demonstrating implicit information flow without explicit parameters.

## Implementation

**Functions:**
- `getline(void)` — reads a line into global `line[]`, returns length
- `copy(void)` — copies `line[]` into `longest[]`
- `main(void)` — coordinates using external variables

**External variables:**
- `char line[MAXLINE]` — current line
- `char longest[MAXLINE]` — longest line found

**Data flow:**
stdin → getline() → line[] (global) + retorna len (explícito)
↓
copy() ← len se pierde (no se pasa)
copy() lee line[], escribe longest[]
↓
main() ← longest[] global


`getline` returns the length, but `copy` does not receive it as a parameter. Length information is **lost as explicit flow** and exists only implicitly inside `line[]` (terminated by `'\0'`).

## External Variables

**Ownership (nonexistent):**

No external variable has an owner. Any function can read or write `line` and `longest` at any time. There is no:
- Single responsible module
- Access control (public to entire file)
- Guarantee of consistent state

The program works because the programmer *commits* that only `getline` writes `line` and only `copy` writes `longest`. The language does not enforce this discipline.

## Comparison

**Extern vs passing arguments:**

| Aspect | Extern | Parameters |
|--------|--------|------------|
| Control flow | Implicit — any function can modify any global | Explicit — data flows through call chain |
| Coupling | Tight — functions depend on variable names | Loose — functions depend only on types |
| Traceability | Hard — who changed `line`? Must inspect all functions | Easy — follow parameters |
| Testing | Globals persist, state leaks | Each call isolated |
| Reusability | Baja — functions tied to global names | High — works with any buffer |

**Why extern exists in K&R:** Extern reflects a programming model where global state was the norm, not the exception. Early C programs were often monolithic: one file, global variables for implicit communication. Explicit parameters were seen as verbose. Extern meant less typing, at the cost of coupling.

## Key Insight

External variables break **local reasoning** — you cannot understand what `copy` does by reading only its body. It reads `line` and writes `longest`, but where do these come from? No parameters tell you.

`getline` returns `len` but it is not propagated, `copy` ignores it. The length travels implicitly inside the array content, not in the visible data flow. Information exists but you cannot see it in function signatures.

## Limitations

What you cannot guarantee with external variables:

- That `longest` contains a valid line before `main` reads it
- That no function accidentally writes to the wrong global
- Thread safety (globals are shared state)
- Any ownership discipline, the language provides none

These are structural weaknesses of the design pattern itself, not bugs in this specific program.

## Notes

- `getline` is a POSIX function name. Rename to `get_line` if needed on modern systems.
- External variables are initialized to zero (unlike locals).
- The `extern` keyword is optional in the file where variables are defined; required in other files.
