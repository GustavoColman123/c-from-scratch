# Notes — Call by Value and Character Arrays

## Source

K&R Chapter 1, sections 1.8 and 1.9.

## Call by value

Every argument in C is passed by value. Always.

The function receives a copy. Whatever it does with that copy
does not affect the variable in the caller.

void double_it(int x)
{
    x = x * 2;  // modifies the copy, not the original
}

This is not a limitation, it is a guarantee.
A function cannot accidentally modify the caller's data
unless it is explicitly given the means to do so.

## Arrays are the apparent exception

When an array is passed to a function, the value copied is the address
of its first element, not the contents of the array.

The function receives a copy of that address.
Through that address, it can read and write the original data directly.

This is still call by value.
What changes is what the value represents.

char s[]   // in a parameter list
char *s    // means exactly this

The two are interchangeable. The array notation is shorthand.

## Strings in C

A string is an array of characters with one requirement:
it must end with the null character `'\0'`.

That terminator is the only way a function knows where the string ends.
Without it, there is no boundary, just memory continuing past the intended content.

`'\0'` has integer value `0`. It is not the character `'0'`, which is `48`.

## How the longest line program is structured

Three functions, one responsibility each:

- `getline` reads one line from input and returns its length.
- `copy` duplicates a string into a destination array.
- `main` tracks the longest line seen and prints it at the end.

No function does more than one thing.
This is what modular design looks like at small scale.

## Open question

`getline` and `copy` both operate on arrays whose size is fixed at declaration.
If the input exceeds that size, the function writes past the end of the array.

I do not know yet how C handles this safely.
The program works on controlled input but the boundary is not enforced.
Leaving this open.
