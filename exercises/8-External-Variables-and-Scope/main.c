#include <stdio.h>

#define MAXLINE 1000

char line[MAXLINE];
char longest[MAXLINE];
int max;
int current_len;

int read_next_line(void);
void commit_to_longest(void);
static void copy(void);

int main(void)
{
    max = 0;
    current_len = 0;
    longest[0] = '\0';

    while ((current_len = read_next_line()) > 0) {
        commit_to_longest();
    }

    if (max > 0) {
        printf("Longest stored line (%d chars):\n%s\n", max, longest);
    } else {
        printf("No input or only empty lines.\n");
    }

    return 0;
}

int read_next_line(void)
{
    int c;
    int i = 0;
    int exit_reason = 0;

    c = getchar();

    while (i < MAXLINE - 1 && c != EOF && c != '\n') {
        line[i++] = c;
        c = getchar();
    }

    if (c == EOF) {
        if (i == 0) {
            return -1;
        }
        exit_reason = 2;
    } else if (c == '\n') {
        exit_reason = 0;
    } else {
        exit_reason = 1;
    }

    if (exit_reason == 1) {
        while ((c = getchar()) != EOF && c != '\n')
            ;
    }

    line[i] = '\0';
    return i;
}

void commit_to_longest(void)
{
    if (current_len > max) {
        max = current_len;
        copy();
    }
}

static void copy(void)
{
    int i = 0;

    while ((longest[i] = line[i]) != '\0') {
        i++;
    }

    line[0] = '\0';
}
