#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Returns number of ulong that represents the matrix rows.
 */
#define num_data(rows) ((rows / ULONG_BITS) + 1)

struct matrix *
matrix_create(ulong rows, ulong cols)
{
	struct matrix *m;

	m = malloc(sizeof(*m));
	m->rows = rows;
	m->cols = cols;
	m->cell = malloc(cols * sizeof(ulong *));

	ulong num_data_rows = num_data(rows);
	for (ulong col = 0; col < cols; col++) {
		m->cell[col] = malloc(num_data_rows * sizeof(ulong));
	}

	return m;
}

void
matrix_destroy(struct matrix *m)
{
	for (ulong col = 0; col < m->cols; col++) {
		free(m->cell[col]);
	}
	free(m->cell);
	free(m);
}

void
matrix_zero(struct matrix *m)
{
	ulong num_data_rows = num_data(m->rows);
	for (ulong col = 0; col < m->cols; col++) {
		for (ulong row = 0; row < num_data_rows; row++) {
			m->cell[col][row] = 0;
		}
	}
}

void
matrix_zero_row(struct matrix *m, ulong row)
{
	for (ulong col = 0; col < m->cols; col++) {
		matrix_reset(m, row, col);
	}
}

void
matrix_add_cols(struct matrix *m, ulong col1, ulong col2)
{
	ulong num_data_rows = num_data(m->rows);
	for (ulong row = 0; row < num_data_rows; row++) {
		m->cell[col1][row] ^= m->cell[col2][row];
	}
}

/*
 * Reference:
 * Cetin K. Kock; Sarath N. Arachchige. A Fast Algorithm for Gaussian
 * Elimination over GF(2) and Its Implementation of the GAPP*. Journal of
 * parallel and distributed computing 13, pages 118-122, 1991.
 */
void
matrix_gauss(struct matrix *m, int *pivots)
{
	ulong pivot;
	int has_pivot;

	for (ulong j = 0; j < m->cols; j++) {
		has_pivot = 0;
		for (ulong i = 0; i < m->rows; i++) {
			if (matrix_get(m, i, j)) {
				pivot = i;
				has_pivot = 1;
				break;
			}
		}

		if (has_pivot) {
			pivots[pivot] = 1;
			for (ulong k = 0; k < m->cols; k++) {
				if (k != j && matrix_get(m, pivot, k)) {
					matrix_add_cols(m, k, j);
				}
			}
		}
	}
}

void
matrix_print(struct matrix *m)
{
	printf("%lu %lu\n", m->rows, m->cols);
	for (ulong row = 0; row < m->rows; row++) {
		for (ulong col = 0; col < m->cols; col++) {
			printf("%d", matrix_get(m, row, col));
		}
		printf("\n");
	}
}

void
matrix_print_pivots(struct matrix *m, int *pivots)
{
	printf("%lu %lu\n", m->rows, m->cols);
	for (ulong row = 0; row < m->rows; row++) {
		printf("%lu | ", row);
		for (ulong col = 0; col < m->cols; col++) {
			printf("%d", matrix_get(m, row, col));
		}
		if (pivots[row]) {
			printf(" *\n");
		} else {
			printf("\n");
		}
	}
}
