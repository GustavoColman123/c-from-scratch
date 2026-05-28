#include <stdio.h>

#define MAX_WIDTH 50

int main(void)
{
    int c;
    int digits[10] = {0};
    int i, j, max = 0;

    while ((c = getchar()) != EOF) {
        if (c >= '0' && c <= '9')
            ++digits[c - '0'];
    }

    for (i = 0; i < 10; ++i)
        if (digits[i] > max)
            max = digits[i];

    printf("Digit Frequency Histogram\n\n");

    for (i = 0; i < 10; ++i) {
        printf("%d | ", i);

        int length = digits[i];
        if (max > MAX_WIDTH)
            length = (digits[i] * MAX_WIDTH) / max;

        for (j = 0; j < length; ++j)
            putchar('*');

        printf(" (%d)\n", digits[i]);
    }

    return 0;
}
