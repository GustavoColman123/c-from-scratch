#include <stdio.h>

static int persistent;

/*
 * Demonstrates how C evaluates:
 * (0 < x) < 10
 */
int chained_comparison(int x)
{
    int temp = (0 < x);
    return temp < 10;
}

int main(void)
{
    /*
     * Section 1: Declarations
     */

    /* int automatic; */
    /* Reading an uninitialized local variable would be undefined behavior */

    int initialized = 42;
    const int constant = 10;

    printf("=== Section 1: Declarations ===\n\n");

    printf("initialized = %d\n", initialized);
    printf("persistent  = %d\n", persistent);
    printf("constant    = %d\n", constant);

    /*
     * Section 2: Arithmetic operators
     */

    int a = 7;
    int b = 3;

    int result = (a / b) * b + (a % b);

    printf("\n=== Section 2: Arithmetic Operators ===\n\n");

    printf("7 / 3   =  %d\n",  7 / 3);
    printf("-7 / 3  = %d\n", -7 / 3);
    printf("7 %% 3   =  %d\n",  7 % 3);
    printf("-7 %% 3  = %d\n", -7 % 3);

    printf("(a/b)*b + (a%%b) = %d\n", result);
    printf("identity holds   = %d\n", result == a);

    /*
     * Section 3: Relational operators
     */

    printf("\n=== Section 3: Relational Operators ===\n\n");

    printf("5 > 3  = %d\n", 5 > 3);
    printf("5 < 3  = %d\n", 5 < 3);
    printf("5 == 5 = %d\n", 5 == 5);
    printf("5 != 5 = %d\n", 5 != 5);

    /*
     * Section 4: Logical operators
     */

    int valid = 1;
    int error = 0;

    printf("\n=== Section 4: Logical Operators ===\n\n");

    printf("valid && error = %d\n", valid && error);
    printf("valid || error = %d\n", valid || error);
    printf("!valid         = %d\n", !valid);
    printf("!error         = %d\n", !error);

    /*
     * Section 5: Chained comparison trap
     */

    int x1 = 20;
    int x2 = 5;

    printf("\n=== Section 5: Chained Comparison Trap ===\n\n");

    printf("x = %d\n", x1);
    printf("0 < x < 10            => %d\n", chained_comparison(x1));
    printf("(0 < x)&&(x < 10)     => %d\n\n",
           (0 < x1) && (x1 < 10));

    printf("x = %d\n", x2);
    printf("0 < x < 10            => %d\n", chained_comparison(x2));
    printf("(0 < x)&&(x < 10)     => %d\n",
           (0 < x2) && (x2 < 10));

    return 0;
}
