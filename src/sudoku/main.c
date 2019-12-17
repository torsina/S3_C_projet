#include <stdlib.h>
#include <stdio.h>

#include <yaml.h>

#include "includes/sudoku.h"

#include "sudoku.inc"
int main(void) {
  Sudoku* test = malloc(sizeof(Sudoku));
  sudoku_get_dim_size(test);
  printf("libYAML version : %s\n", yaml_get_version_string());
  return EXIT_SUCCESS;
}
