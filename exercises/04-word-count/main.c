#include <stdio.h>

#define IN 1
#define OUT 0

int main(void)
{
    int c;
    int words = 0;
    int state = OUT;

    while ((c = getchar()) != EOF)
    {
        if (c == ' ' || c == '\n' || c == '\t')
        {
            state = OUT;
        }
        else if (state == OUT)
        {
            state = IN;
            words++;
        }
    }
    printf("%d\n", words);
    return 0;
}
