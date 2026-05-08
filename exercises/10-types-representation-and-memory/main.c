#include <stdio.h>
#include <limits.h>
#include <float.h>

#define ASCII_ZERO     '0'
#define OCTAL_EXAMPLE  012
#define HEX_EXAMPLE    0xFF

int main(void)
{
    unsigned char a = 200;
    unsigned char b = 100;
    unsigned char c;

    const char *p = "hello";
    char        s[] = "hello";

    c = a + b;

    /*
     * Section 1: Constants and their values
     */

    printf("=== Section 1: Constants ===\n\n");

    printf("'0'       = %d  (digit character in execution character set)\n", '0');
    printf("0         = %d  (integer zero)\n", 0);
    printf("'\\0'      = %d  (null terminator, integer value zero)\n", '\0');
    printf("\"0\"       = string containing '0' and implicit terminator\n");
    printf("\"\\0\"      = string with explicit '\\0' plus implicit terminator\n");

    printf("\n");
    printf("ASCII_ZERO    = %d\n",  ASCII_ZERO);
    printf("OCTAL_EXAMPLE = %d  (012 in octal)\n",  OCTAL_EXAMPLE);
    printf("HEX_EXAMPLE   = %d  (0xFF in hex)\n",   HEX_EXAMPLE);

    printf("\nsizeof('A')  = %zu  (character constant has type int in C)\n", sizeof('A'));
    printf("sizeof(\"A\")  = %zu  (array: 'A' plus '\\0')\n", sizeof("A"));

    /*
     * Section 2: sizeof basic types
     */

    printf("\n=== Section 2: Type sizes ===\n\n");

    printf("sizeof(char)        = %zu\n", sizeof(char));
    printf("sizeof(short)       = %zu\n", sizeof(short));
    printf("sizeof(int)         = %zu\n", sizeof(int));
    printf("sizeof(long)        = %zu\n", sizeof(long));
    printf("sizeof(long long)   = %zu\n", sizeof(long long));
    printf("sizeof(float)       = %zu\n", sizeof(float));
    printf("sizeof(double)      = %zu\n", sizeof(double));
    printf("sizeof(long double) = %zu\n", sizeof(long double));

    /*
     * Section 3: Limits
     */

    printf("\n=== Section 3: Limits ===\n\n");

    printf("SCHAR_MIN = %d\n",  SCHAR_MIN);
    printf("SCHAR_MAX = %d\n",  SCHAR_MAX);
    printf("UCHAR_MAX = %u\n",  UCHAR_MAX);
    printf("CHAR_MIN  = %d\n",  CHAR_MIN);
    printf("CHAR_MAX  = %d\n",  CHAR_MAX);
    printf("INT_MIN   = %d\n",  INT_MIN);
    printf("INT_MAX   = %d\n",  INT_MAX);
    printf("UINT_MAX  = %u\n",  UINT_MAX);
    printf("LONG_MAX  = %ld\n", LONG_MAX);
    printf("LLONG_MAX = %lld\n", LLONG_MAX);

    /*
     * Section 4: Integer promotions
     *
     * a and b are promoted to int before addition.
     * 200 + 100 = 300 computed in int — no overflow.
     * Assignment to unsigned char truncates: 300 % 256 = 44.
     */

    printf("\n=== Section 4: Integer promotions ===\n\n");

    printf("a          = %u  (unsigned char)\n", a);
    printf("b          = %u  (unsigned char)\n", b);
    printf("a + b      = %d  (promoted to int: 300, no overflow)\n", a + b);
    printf("c = a + b  = %u  (truncated to unsigned char: 300 %% 256 = 44)\n", c);

    /*
     * Section 5: Pointer vs array
     *
     * p holds the address of a string literal.
     * s is the array itself, initialized from the same content.
     * sizeof(p) is the size of the pointer.
     * sizeof(s) includes all 6 bytes: 5 chars + '\0'.
     */

    printf("\n=== Section 5: Pointer vs array ===\n\n");

    printf("const char *p = \"hello\"\n");
    printf("sizeof(p) = %zu  (size of pointer)\n", sizeof(p));

    printf("\nchar s[] = \"hello\"\n");
    printf("sizeof(s) = %zu  (size of array including '\\0')\n", sizeof(s));

    printf("\np points to a string literal with static storage duration.\n");
    printf("Modifying it would be undefined behavior.\n");
    printf("s is a mutable copy on the stack.\n");
    printf("Same content. Different memory. Different rules.\n");

    return 0;
}
