#include "flags.h"

static unsigned flags = 0;

static int valid_bit(unsigned bit)
{
    return bit < FLAGS_MAX_BITS;
}

static unsigned mask_for(unsigned bit)
{
    return 1u << bit;
}

void flags_reset(void)
{
    flags = 0;
}

int flags_set(unsigned bit)
{
    if (!valid_bit(bit)) {
        return 0;
    }

    flags |= mask_for(bit);
    return 1;
}

int flags_clear(unsigned bit)
{
    if (!valid_bit(bit)) {
        return 0;
    }

    flags &= ~mask_for(bit);
    return 1;
}

int flags_toggle(unsigned bit)
{
    if (!valid_bit(bit)) {
        return 0;
    }

    flags ^= mask_for(bit);
    return 1;
}

int flags_is_set(unsigned bit)
{
    if (!valid_bit(bit)) {
        return 0;
    }

    return (flags & mask_for(bit)) != 0;
}

unsigned flags_snapshot(void)
{
    return flags;
}
