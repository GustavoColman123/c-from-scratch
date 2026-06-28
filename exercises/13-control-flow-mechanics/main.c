#include <stdio.h>

int main(void)
{
    /*
     * Section 1: Statements and blocks
     */
    int x = 3;

    printf("=== Section 1: Statements and Blocks ===\n\n");

    if (x > 0) {
        int temp = x * 2;

        printf("x is positive\n");
        printf("temporary block value = %d\n", temp);
    }

    /*
     * Section 2: If-else and braces
     */
    int a = 0;
    int b = 1;

    printf("\n=== Section 2: If-Else and Braces ===\n\n");

    if (a > 0) {
        if (b > 0) {
            printf("both are positive\n");
        }
    } else {
        printf("a is not positive\n");
    }

    /*
     * Section 3: Else-if chain
     */
    int score = 85;
    char grade;

    printf("\n=== Section 3: Else-If Chain ===\n\n");

    if (score >= 90) {
        grade = 'A';
    } else if (score >= 80) {
        grade = 'B';
    } else if (score >= 70) {
        grade = 'C';
    } else {
        grade = 'F';
    }

    printf("score = %d\n", score);
    printf("grade = %c\n", grade);

    /*
     * Section 4: Switch
     */
    char c = '2';
    int digit = 0;
    int other = 0;

    printf("\n=== Section 4: Switch ===\n\n");

    switch (c) {
    case '0':
    case '1':
    case '2':
        digit++;
        break;
    default:
        other++;
        break;
    }

    printf("character = %c\n", c);
    printf("digit count = %d\n", digit);
    printf("other count = %d\n", other);

    /*
     * Section 5: While loop
     */
    int i = 0;
    int n = 4;
    int while_sum = 0;

    printf("\n=== Section 5: While Loop ===\n\n");

    while (i < n) {
        while_sum += i;
        i++;
    }

    printf("sum using while = %d\n", while_sum);

    /*
     * Section 6: For loop
     */
    int for_sum = 0;

    printf("\n=== Section 6: For Loop ===\n\n");

    for (i = 0; i < n; i++) {
        for_sum += i;
    }

    printf("sum using for   = %d\n", for_sum);

    /*
     * Section 7: Do-while loop
     */
    int number = 0;
    int digits = 0;

    printf("\n=== Section 7: Do-While Loop ===\n\n");

    do {
        digits++;
        number /= 10;
    } while (number > 0);

    printf("digits produced for zero = %d\n", digits);

    /*
     * Section 8: Break and continue
     */
    int values[] = {3, -1, 7, 9};
    int target = 7;
    int found_index = -1;

    printf("\n=== Section 8: Break and Continue ===\n\n");

    for (i = 0; i < 4; i++) {
        if (values[i] < 0) {
            continue;
        }

        if (values[i] == target) {
            found_index = i;
            break;
        }
    }

    printf("target = %d\n", target);
    printf("found index = %d\n", found_index);

    return 0;
}
