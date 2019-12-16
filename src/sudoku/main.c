//
// Created by Tim on 08/12/2019.
//

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ga.h>
#include "includes/sudoku.h"

#include "sudoku.inc"

unsigned int sudoku_get_dim_size(Sudoku* sudoku) {
  return sudoku->dim_size;
}

unsigned int evaluate(unsigned int* individual, const void* sudoku) {
  sudoku = (Sudoku*)sudoku;
  unsigned int score = 0;
  // keep a state of every column and row already checked
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  bool state_rows = ga_malloc(dim_size);
  bool state_columns = ga_malloc(dim_size);
  // read left to right
  unsigned int index = 0;
  while(index < pow(dim_size, 2)) {
    index++;
  }
}

unsigned int* _evaluate_merge_problem_solution(unsigned int* individual, Sudoku* sudoku) {
  size_t size = pow(sudoku_get_dim_size(sudoku), 2);
  unsigned int* merge = ga_malloc(sizeof(unsigned int) * size);
  for(unsigned int i = 0; i < size; i++) {
    if(sudoku->problem[i] != NULL) merge[i] = (unsigned int)sudoku->problem[i];
    else merge[i] = individual[i];
  }
  return merge;
}

bool _evaluate_check_row(unsigned int* individual, Sudoku* sudoku, unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int rowIndex = index / dim_size;
  unsigned int start = rowIndex * dim_size;
  unsigned int end = rowIndex * (dim_size + 1);
  // max int for a value = dim_size - 1
  unsigned int used_values = malloc(sizeof(unsigned int) * (dim_size - 1));
  unsigned int i = start;
  bool result = true;
  while(i < end && result) {
    i++;
  }
}

bool _evaluate_check_column() {

}

bool _evaluate_add_used_number(unsigned int* used_values, unsigned int used_size, unsigned int new_value) {
  unsigned int i = 0;
  bool result = true;
  while(i < used_size && result) {
    if(used_values[i] == new_value) result = false;
    i++;
  }
  used_values[used_size] = new_value;
  return result;
}
