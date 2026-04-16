# Problem 08 — Call by Value and Character Arrays

## Mistake 1 — Expecting a scalar argument to change in the caller

**What I wrote**
void reset(int x)
{
    x = 0;
}

reset(length);  // expected length to become 0

**What I thought**
Passing a variable to a function gives the function access to it.

**What actually happens**
The function receives a copy. `x` inside `reset` is a separate variable.
`length` in the caller is untouched. No warning, no error.

**Fix**
Return the new value, or pass a pointer.
For this program, `getline` returns the length explicitly — the caller stores it.


## Mistake 2 — Forgetting the null terminator when copying

**What I wrote**
void copy(char to[], char from[], int len)
{
    int i;
    for (i = 0; i < len; ++i)
        to[i] = from[i];
}

**What I thought**
Copying the characters is enough.

**What actually happens**
`to` has no terminator. `printf("%s", to)` reads past the intended end
into whatever memory follows. Output is garbage or the program crashes.

**Fix**
to[i] = '\0';  // after the loop
The terminator is not optional. It is the only boundary a string has.

## Mistake 3 — Assuming the array is copied into the function

**What I thought**
void copy(char to[], char from[], int len)
`from` is a local copy of the array. Modifying it inside the function is safe.

**What actually happens**
`from` is the address of the first element of the original array.
Any write to `from[i]` modifies the caller's data directly.
The copy is of the address, not the contents.

**No fix needed here, but the mental model was wrong.**
The correct model: `char from[]` in a parameter list means `char *from`.
The array notation is shorthand for a pointer.


## Mistake 4 — Not checking if the line exceeded MAXLINE

**What I thought**
Input lines will fit. The buffer is large enough.

**What actually happens**
`getline` stops writing at `MAXLINE - 1` and appends `'\0'`.
The rest of the line is still in the input buffer, unread.
The length returned reflects only what was stored, not the full line.
A line longer than `MAXLINE` gets silently truncated with no indication.

**Fix**
Compare the returned length against `MAXLINE`.
If it equals `MAXLINE - 1`, the line may have been truncated.
The program as written does not handle this — it is a known limitation.

## Mistake 5 — Using `=` to compare the null terminator

**What I wrote**
while (from[i] = '\0')  // intended to stop at null terminator

**What I thought**
`=` checks equality here.

**What actually happens**
`=` is assignment, not comparison. This assigns `'\0'` (value 0) to `from[i]`
and evaluates to `0` — so the loop never executes.
The compiler may warn. The behavior is clearly wrong.

**Fix**
while (from[i] != '\0')

Or equivalently, since `'\0'` is `0`:
while (from[i])
