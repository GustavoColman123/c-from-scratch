# Power Function

## Description

Implements a function `power(base, n)` that raises an integer
to a non-negative integer exponent using iterative multiplication.

## Concepts Covered

- Function definition
- Function prototypes
- Parameter passing (pass-by-value)
- Local scope and lifetime of variables
- Return values
- Separation of interface and implementation

## Approach

The program defines a function `power` that multiplies the base
by itself `n` times using a loop. The function is declared with a
prototype before `main`, allowing the compiler to verify correct
usage before the actual definition.

## Example Output
0 1 1
1 2 -3
2 4 9
3 8 -27
4 16 81
...
