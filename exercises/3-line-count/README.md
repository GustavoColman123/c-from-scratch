# Line Counting

## Description

Counts the number of newline characters (`'\n'`) from stdin until EOF.

The program counts delimiters, not logical lines.
A trailing line without `'\n'` is not counted.

## Approach

The input is processed as a stream of characters.
Each character is read with `getchar()` and tested against `'\n'`.
Only matching characters increment the counter.

Unlike character counting, the value must be inspected —
so it has to be stored.

## Key Insight

`getchar()` returns `int`, not `char`.

EOF is typically `−1`, outside the range of valid character values.
A `char` variable cannot reliably hold it on all systems.
The type exists for correct control flow, not for character storage.

## Operator Precedence Trap

while (c = getchar() != EOF)

`!=` binds tighter than `=`, so this is interpreted as:
c = (getchar() != EOF)

c receives `0` or `1`. The character is discarded.
The compiler does not warn. The program counts nothing.

The correct form:

while ((c = getchar()) != EOF)

## Limitation

If the input does not end with `'\n'`, the last line is not counted.
The program counts newline delimiters, not logical lines.
`hello\nworld` returns `1`.
