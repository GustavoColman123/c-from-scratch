#include <stdio.h>

int main(void)
{
    /*
     * Section 1: Type conversions
     */
    int integer_value = 7;
    double converted_value = integer_value;
    double late_conversion = 7 / 3;
    double early_conversion = (double) 7 / 3;

    printf("=== Section 1: Type Conversions ===\n\n");

    printf("integer_value                 = %d\n", integer_value);
    printf("converted_value               = %.2f\n", converted_value);
    printf("7 / 3 stored in double         = %.2f\n", late_conversion);
    printf("(double) 7 / 3                 = %.2f\n", early_conversion);

    /*
     * Section 2: Increment and decrement
     */
    int n = 5;
    int x = n++;

    printf("\n=== Section 2: Increment and Decrement ===\n\n");

    printf("after x = n++  -> x = %d, n = %d\n", x, n);

    n = 5;
    x = ++n;

    printf("after x = ++n  -> x = %d, n = %d\n", x, n);

    /*
     * Section 3: Bitwise operators
     */
    unsigned flags = 0;
    unsigned ready = 1u << 0;
    unsigned error = 1u << 1;
    unsigned debug = 1u << 2;

    printf("\n=== Section 3: Bitwise Operators ===\n\n");

    flags |= ready;
    flags |= debug;

    printf("after setting READY and DEBUG  = %u\n", flags);
    printf("READY is set                   = %u\n", (flags & ready) != 0);
    printf("ERROR is set                   = %u\n", (flags & error) != 0);

    flags ^= debug;

    printf("after toggling DEBUG           = %u\n", flags);

    flags &= ~ready;

    printf("after clearing READY           = %u\n", flags);

    /*
     * Section 4: Assignment operators
     */
    int total = 10;

    printf("\n=== Section 4: Assignment Operators ===\n\n");

    printf("initial total                  = %d\n", total);

    total += 5;
    printf("after total += 5               = %d\n", total);

    total *= 2;
    printf("after total *= 2               = %d\n", total);

    total >>= 1;
    printf("after total >>= 1              = %d\n", total);

    /*
     * Section 5: Conditional expression
     */
    int a = 12;
    int b = 20;
    int max = (a > b) ? a : b;

    printf("\n=== Section 5: Conditional Expression ===\n\n");

    printf("a                              = %d\n", a);
    printf("b                              = %d\n", b);
    printf("max                            = %d\n", max);
    printf("status                         = %s\n",
           (max > 15) ? "large" : "small");

    return 0;
}
