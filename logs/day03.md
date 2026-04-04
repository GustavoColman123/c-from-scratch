## Log

**First attempt**

Wrote `char c` out of habit. Compiled fine.
Remembered the EOF problem from earlier and changed it to `int` before testing.
Small win.

**Second attempt**

Forgot the extra parentheses in the while condition.
`c` was always `0` or `1`. Counted zero lines on every input.
Took me longer than I want to admit to spot it.

**What made it click**

The difference from Day 02 is not the loop — it is the question the loop asks.
Day 02: is there still input?
Day 03: what is this character?

Once I saw it that way, the structure made sense immediately.

**Unexpected realization**

`'\n'` is not special syntax. It is just the integer value `10`.
The `if` is comparing two integers, nothing more.
The character notation is for readability, not for meaning.

**Still not fully comfortable with**

Operator precedence in expressions that mix `=` and `!=`.
I understand the rule. I do not yet trust my instinct on it.
Will probably need to burn myself again before it becomes automatic.
