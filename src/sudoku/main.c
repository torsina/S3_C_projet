#include <stdlib.h>
#include "includes/sudoku.h"
#include "sudoku.inc"
int main(void) {
  Sudoku* test = malloc(sizeof(Sudoku));
  sudoku_get_dim_size(test);
  return EXIT_SUCCESS;
}
