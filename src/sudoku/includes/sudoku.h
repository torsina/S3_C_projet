#include <ga.h>

#ifndef SRC_SUDOKU_INCLUDES_SUDOKU_H_
#define SRC_SUDOKU_INCLUDES_SUDOKU_H_

typedef struct _Sudoku Sudoku;

unsigned int sudoku_get_dim_size(Sudoku* sudoku);

extern unsigned int evaluate(unsigned int* individual, const void* sudoku);

#endif  // SRC_SUDOKU_INCLUDES_SUDOKU_H_
