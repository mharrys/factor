#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "config.h"

/*
 * Binary matrix with bit indexing. Implemented as column major to speed up
 * gauss elimination over GF(2).
 */
struct matrix {
	ulong	rows;
	ulong	cols;
	ulong	**cell;
};

struct matrix *
matrix_create(ulong rows, ulong cols);

void
matrix_destroy(struct matrix *m);

inline int
matrix_get(struct matrix *m, ulong row, ulong col)
{
	return (m->cell[col][row / ULONG_BITS] & (1L << row % ULONG_BITS)) > 0;
}

inline void
matrix_set(struct matrix *m, ulong row, ulong col)
{
	m->cell[col][row / ULONG_BITS] |= (1L << row % ULONG_BITS);
}

inline void
matrix_flip(struct matrix *m, ulong row, ulong col)
{
	m->cell[col][row / ULONG_BITS] ^= (1L << row % ULONG_BITS);
}

inline void
matrix_reset(struct matrix *m, ulong row, ulong col)
{
	m->cell[col][row / ULONG_BITS] &= ~(1L << row % ULONG_BITS);
}

void
matrix_zero(struct matrix *m);

void
matrix_zero_row(struct matrix *m, ulong row);

void
matrix_zero_col(struct matrix *m, ulong col);

/*
 * col1 = col1 ^ col2
 */
void
matrix_add_cols(struct matrix *m, ulong col1, ulong col2);

void
matrix_gauss(struct matrix *m, int *pivots);

void
matrix_print(struct matrix *m);

void
matrix_print_pivots(struct matrix *m, int *pivots);

#endif
