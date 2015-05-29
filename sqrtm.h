#ifndef SQRTM_H_INCLUDED
#define SQRTM_H_INCLUDED

#include "config.h"

/*
 * Solves the modular square root r^2 = n mod p using Tonelli-Shanks algorithm.
 */
void
sqrtm(mpz_t r, mpz_t n, mpz_t p);

#endif
