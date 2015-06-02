#ifndef SMOOTH_H_INCLUDED
#define SMOOTH_H_INCLUDED

#include "config.h"

/*
 * Return computed smoothness bound from n and c using a magic formula.
 */
ulong
smooth(mpz_t n, float c);

#endif
