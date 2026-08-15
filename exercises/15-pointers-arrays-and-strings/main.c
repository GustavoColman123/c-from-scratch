#include <stddef.h>
#include <stdio.h>

static void wrong_swap(int x, int y)
{
    int temp;

    temp = x;
    x = y;
    y = temp;
}

static void swap_values(int *px, int *py)
{
    int temp;

    temp = *px;
    *px = *py;
    *py = temp;
}

static int read_constant(int *out)
{
    if (out == NULL) {
        return 0;
    }

    *out = 42;
    return 1;
}

static int sum_index(const int values[], size_t length)
{
    int sum = 0;

    for (size_t i = 0; i < length; i++) {
        sum += values[i];
    }

    return sum;
}

static int sum_pointer(const int *values, size_t length)
{
    int sum = 0;
    const int *p = values;
    const int *end = values + length;

    while (p < end) {
        sum += *p;
        p++;
    }

    return sum;
}

static size_t length_index(const char s[])
{
    size_t length = 0;

    while (s[length] != '\0') {
        length++;
    }

    return length;
}

static size_t length_pointer(const char *s)
{
    const char *start = s;

    while (*s != '\0') {
        s++;
    }

    return (size_t)(s - start);
}

static void copy_string(char *dest, const char *src)
{
    while ((*dest++ = *src++) != '\0') {
        ;
    }
}

static int compare_string(const char *s, const char *t)
{
    for (; *s == *t; s++, t++) {
        if (*s == '\0') {
            return 0;
        }
    }

    return (unsigned char)*s - (unsigned char)*t;
}

static const char *find_char(const char *s, char target)
{
    while (*s != '\0') {
        if (*s == target) {
            return s;
        }

        s++;
    }

    return NULL;
}

static void print_array(const int values[], size_t length)
{
    printf("values:");

    for (size_t i = 0; i < length; i++) {
        printf(" %d", values[i]);
    }

    printf("\n");
}

int main(void)
{
    printf("=== Day 15: Pointers, Arrays, and Strings ===\n\n");

    printf("=== Section 1: Address and Indirection ===\n\n");

    int x = 10;
    int *p = &x;

    printf("x before = %d\n", x);
    printf("*p reads = %d\n", *p);
    printf("p points to x = %s\n", p == &x ? "yes" : "no");

    *p = 99;

    printf("x after *p = 99 -> %d\n\n", x);

    printf("=== Section 2: Swap and Pass-by-Address ===\n\n");

    int a = 3;
    int b = 7;

    printf("before wrong_swap: a = %d, b = %d\n", a, b);
    wrong_swap(a, b);
    printf("after wrong_swap:  a = %d, b = %d\n", a, b);

    swap_values(&a, &b);
    printf("after swap_values: a = %d, b = %d\n\n", a, b);

    printf("=== Section 3: Output Pointer ===\n\n");

    int output = 0;

    if (read_constant(&output)) {
        printf("read_constant succeeded: output = %d\n", output);
    }

    if (!read_constant(NULL)) {
        printf("read_constant rejected a null output pointer\n\n");
    }

    printf("=== Section 4: Arrays and Pointer Arithmetic ===\n\n");

    int values[] = {10, 20, 30, 40, 50};
    size_t length = sizeof values / sizeof values[0];

    print_array(values, length);

    printf("sum using index   = %d\n", sum_index(values, length));
    printf("sum using pointer = %d\n", sum_pointer(values, length));

    printf("values[2]       = %d\n", values[2]);
    printf("*(values + 2)   = %d\n", *(values + 2));

    const int *cursor = values;
    const int *end = values + length;

    printf("pointer walk:");

    while (cursor < end) {
        printf(" %d", *cursor);
        cursor++;
    }

    printf("\n");

    printf("one-past pointer created safely = %s\n\n",
           end == values + length ? "yes" : "no");

    printf("=== Section 5: Subarray View ===\n\n");

    const int *subarray = values + 2;
    size_t subarray_length = length - 2;

    printf("subarray[0] points to original values[2] = %d\n", subarray[0]);
    printf("subarray sum from values + 2 = %d\n\n",
           sum_pointer(subarray, subarray_length));

    printf("=== Section 6: Character Arrays and String Literals ===\n\n");

    char message_array[] = "now is the time";
    const char *message_literal = "now is the time";

    printf("modifiable array before = %s\n", message_array);

    message_array[0] = 'N';

    printf("modifiable array after  = %s\n", message_array);
    printf("string literal view     = %s\n\n", message_literal);

    printf("=== Section 7: String Traversal ===\n\n");

    const char *text = "kernel";

    printf("text = %s\n", text);
    printf("length using index   = %zu\n", length_index(text));
    printf("length using pointer = %zu\n\n", length_pointer(text));

    printf("=== Section 8: String Copy and Compare ===\n\n");

    char buffer[32];

    copy_string(buffer, "kernel flags");

    printf("copied string = %s\n", buffer);
    printf("compare(\"abc\", \"abc\") = %d\n", compare_string("abc", "abc"));
    printf("compare(\"abc\", \"abd\") = %d\n", compare_string("abc", "abd"));
    printf("compare(\"abd\", \"abc\") = %d\n\n", compare_string("abd", "abc"));

    printf("=== Section 9: Pointer Search ===\n\n");

    const char *word = "bootloader";
    const char *found = find_char(word, 'l');

    if (found != NULL) {
        printf("first 'l' offset in %s = %td\n", word, found - word);
    }

    found = find_char(word, 'x');

    if (found == NULL) {
        printf("'x' was not found in %s\n", word);
    }

    return 0;
}
