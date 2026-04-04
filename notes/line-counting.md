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
