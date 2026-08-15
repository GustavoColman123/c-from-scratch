#include <stdio.h>

#include "flags.h"

static void print_flag(const char *name, unsigned bit)
{
    printf("%s = %s\n", name, flags_is_set(bit) ? "on" : "off");
}

static void print_status(const char *label)
{
    printf("%s\n", label);
    printf("raw flags = %u\n", flags_snapshot());

    print_flag("READY", FLAGS_READY);
    print_flag("ERROR", FLAGS_ERROR);
    print_flag("DEBUG", FLAGS_DEBUG);

    printf("\n");
}

int main(void)
{
    printf("=== Day 14: Functions and Program Structure ===\n\n");

    print_status("Initial state");

    flags_set(FLAGS_READY);
    flags_set(FLAGS_DEBUG);

    print_status("After setting READY and DEBUG");

    flags_toggle(FLAGS_DEBUG);

    print_status("After toggling DEBUG");

    flags_clear(FLAGS_READY);

    print_status("After clearing READY");

    if (!flags_set(99u)) {
        printf("invalid bit rejected\n\n");
    }

    flags_reset();

    print_status("After reset");

    return 0;
}
