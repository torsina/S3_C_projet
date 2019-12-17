#include "includes/sudoku.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ga.h>

#include "sudoku.inc"

unsigned int sudoku_get_dim_size(const Sudoku* sudoku) {
  assert(sudoku);
  return sudoku->dim_size;
}

unsigned int* _evaluate_merge_problem_solution(unsigned int* individual,
                                               const Sudoku* sudoku) {
  if (!individual) {
    return NULL;
  }

  if (!sudoku) {
    return NULL;
  }

  // A sudoku is a grid make of n*n squares of n*n tiles (n² tiles per square).
  unsigned int n = sudoku_get_dim_size(sudoku);
  // The size of one square.
  size_t size = n * n;

  unsigned int* merge = ga_malloc(sizeof(unsigned int) * size);

  if (merge) {
    for (unsigned int i = 0; i < size; i++) {
      /* If the tile in the sudoku grid (the problem loaded from the file)
       * contains a number that we need to check against :
       */
      if (sudoku->problem[i] != 0) {
        merge[i] = (unsigned int)sudoku->problem[i];
      } else {
        merge[i] = individual[i] + 1;
      }
    }
    return merge;
  } else {
    return NULL;
  }
}

unsigned int _evaluate_used_numbers_duplicates(unsigned int* used_values,
                                               unsigned int used_size) {
  assert(used_values);
  assert(used_size);
  assert(used_size > 0);

  unsigned int unique_values[used_size];
  unsigned int unique_size = 0;
  unsigned int duplicates = 0;
  for (unsigned int i = 0; i < used_size; i++) {
    bool result = true;
    unsigned int j = 0;
    while (j < unique_size && result) {
      if (unique_values[j] == used_values[i])
        result = false;
      else
        unique_values[unique_size] = used_values[i];
      j++;
    }
    if (!result)
      duplicates++;
  }
  return duplicates;
}

unsigned int _evaluate_check_row(unsigned int* merge, const Sudoku* sudoku,
                                 unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int rowIndex = index / dim_size;
  unsigned int start = rowIndex * dim_size;
  unsigned int end = rowIndex * (dim_size + 1);
  // max int for a value = dim_size - 1
  unsigned int* used_values = malloc(sizeof(unsigned int) * (dim_size - 1));
  unsigned int used_size = 0;
  for (unsigned int i = start; i < end; i++) {
    used_values[used_size] = merge[i];
    used_size++;
  }
  unsigned int duplicates =
      _evaluate_used_numbers_duplicates(used_values, used_size);
  free(used_values);
  return duplicates;
}

bool _evaluate_check_column(unsigned int* merge, const Sudoku* sudoku,
                            unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int columnIndex = index % dim_size;
  // max int for a value = dim_size - 1
  unsigned int* used_values = malloc(sizeof(unsigned int) * (dim_size - 1));
  unsigned int used_size = 0;
  for (unsigned int i = columnIndex; i < pow(dim_size, 2); i += dim_size) {
    used_values[used_size] = merge[i];
    used_size++;
  }
  unsigned int duplicates =
      _evaluate_used_numbers_duplicates(used_values, used_size);
  free(used_values);
  return duplicates;
}

unsigned int evaluate(unsigned int* individual, const void* sudoku) {
  assert(individual);
  assert(sudoku);

  sudoku = (Sudoku*)sudoku;
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int score = pow(dim_size, 2);
  unsigned int* merge = _evaluate_merge_problem_solution(individual, sudoku);
  unsigned int duplicates = 0;
  // rows
  for (unsigned int i = 0; i < pow(dim_size, 2); i += dim_size) {
    duplicates += _evaluate_check_row(merge, sudoku, i);
  }
  // columns
  for (unsigned int i = 0; i < dim_size; i++) {
    duplicates += _evaluate_check_column(merge, sudoku, i);
  }
  free(merge);
  // afraid that score could be negative if too much duplicates(because an error
  // can be counted multiple times) so squaring score to be sure
  return score * score - duplicates;
}
