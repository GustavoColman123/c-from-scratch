# Longest Line — Call by Value and Character Arrays

## Description

Implements a program that reads input lines and prints the longest one.
The program demonstrates two fundamental concepts of C:

1. Arguments are passed by value.
2. Character arrays (strings) allow functions to modify data through pointers.

## Concepts Covered

- Function prototypes
- Call by value semantics
- Arrays as function parameters
- Null-terminated strings (`'\0'`)
- Separation of responsibilities between functions
- Modular program design

## Program Structure

- `getline` reads a line into a character array and returns its length.
- `copy` copies one string into another.
- `main` coordinates the process and prints the longest line.

## Key Insight

Although C uses call by value, passing an array allows the function
to modify the original data because the copied value is the address
of the array's first element.
