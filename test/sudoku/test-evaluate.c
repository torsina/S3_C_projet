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
  sudoku_get_dim_size(test);
  sudoku_destroy(test);
  return EXIT_SUCCESS;
}
