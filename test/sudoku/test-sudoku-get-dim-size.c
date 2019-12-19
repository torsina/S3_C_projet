#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/sudoku/includes/sudoku.h"

#include "../../src/sudoku/sudoku.inc"

int main(void) {
  const unsigned int DIM_SIZE = 5;
  Sudoku* sudoku = sudoku_create(DIM_SIZE);
  assert(sudoku);
  assert(sudoku_get_dim_size(sudoku) == DIM_SIZE);
  sudoku->dim_size = 0;
  assert(sudoku_get_dim_size(sudoku) == 0);
  sudoku_destroy(sudoku);
  return EXIT_SUCCESS;
}
