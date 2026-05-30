# Notes — Line Counting

## Source

K&R Chapter 1, section 1.5.3.
## What it actually is

Counts occurrences of the newline character (`'\n'`) in the input stream
until EOF.

The program counts delimiters, not logical lines.
This distinction is minor in most cases and significant in edge cases.

## What changed from character counting

One condition inside the loop body.
Everything else is identical.

The stream is consumed the same way.
The difference is that now the value matters,
so the character must be stored.

This is the first program where the loop separates responsibilities:

- controlling when to stop (EOF)
- deciding which inputs matter (`'\n'`)

## The question the loop asks

Character counting asks: does input continue?

Line counting asks:
- does input continue?
- if so, does this character match a condition?

That second question makes storing the value necessary.

## Why '\n' is not special

`'\n'` is an integer value defined by the execution environment.

The comparison is between integer values, not special "characters".
The notation exists for readability.
## The precedence trap

while (c = getchar() != EOF)
Looks correct. Is not.

!= binds tighter than =, so the expression becomes:

c = (getchar() != EOF)

c receives 0 or 1.
The character is discarded before it can be compared.

The program compiles and runs, but the logic is incorrect.
The comparison with '\n' never succeeds.

## Open question

If the input does not end with '\n', the last line is not counted.
A correct implementation would need to detect whether the last character
before EOF was not a newline.

This likely requires tracking previous input or maintaining state.

Leaving this open until more control structures are introduced.

## Connection to later topics

This is the first filtering pattern in the book:
read everything, act on some of it.

Word counting will likely extend this idea,
not by filtering individual characters,
but by detecting transitions between states.

## Observation

K&R introduces three programs in sequence:

Copy: iteration over a stream
Character counting: accumulation
Line counting: conditional filtering

Each program introduces exactly one new idea.

The progression is deliberate.
Each step builds directly on the previous one.
