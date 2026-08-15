#ifndef FLAGS_H
#define FLAGS_H

#define FLAGS_MAX_BITS 8u

#define FLAGS_READY 0u
#define FLAGS_ERROR 1u
#define FLAGS_DEBUG 2u

void flags_reset(void);

int flags_set(unsigned bit);
int flags_clear(unsigned bit);
int flags_toggle(unsigned bit);
int flags_is_set(unsigned bit);

unsigned flags_snapshot(void);

#endif
