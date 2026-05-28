#include <stdio.h>

int main(void)
{
    int c;
    int digits[10];
    int whitespace, other;
    int i;

    whitespace = other = 0;

    for (i = 0; i < 10; ++i)
        digits[i] = 0;

    while ((c = getchar()) != EOF) {
        if (c >= '0' && c <= '9')
            ++digits[c - '0'];
        else if (c == ' ' || c == '\t' || c == '\n')
            ++whitespace;
        else
            ++other;
    }

    printf("digits:\n");
    for (i = 0; i < 10; ++i)
        if (digits[i] > 0)
            printf("  %d: %d\n", i, digits[i]);

    printf("whitespace: %d\n", whitespace);
    printf("other:      %d\n", other);

    return 0;
}
