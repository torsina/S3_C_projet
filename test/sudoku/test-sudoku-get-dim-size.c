//
// Created by Aelis on 17/12/2019.
//

#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/sudoku/includes/sudoku.h"
#include "../../src/sudoku/sudoku.inc"

int main(void) {
  Sudoku* test = malloc(sizeof(Sudoku));
  test->dim_size = 5;
  assert(sudoku_get_dim_size(test) == 5);
  test->dim_size = 0;
  assert(sudoku_get_dim_size(test) != 5);
  sudoku_destroy(test);
  return EXIT_SUCCESS;
}
