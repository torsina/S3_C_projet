#include "includes/sudoku.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ga.h>

#include "sudoku.inc"

/**
 * \brief Returns the number of tiles in one square in the Sudoku grid.
 *
 * If a square has a width (and length) of n tiles, it contains nxn tiles.
 * There are nxn tiles in a Sudoku grid. The total number of tiles is :
 * N=(nxn)x(nxn)
 *
 * dim_size corresponds to nxn.
 * Example : in a "regular" sudoku, squares are made of 3x3 tiles.
 * dim_size = 9.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int sudoku_get_dim_size(const Sudoku* sudoku)
 * \param sudoku a pointer to the Sudoku to get the size from.
 * \return The number of tiles in one square (nxn).
 * \sa _evaluate_merge_problem_solution
 */
unsigned int sudoku_get_dim_size(const Sudoku* sudoku) {
  assert(sudoku);
  return sudoku->dim_size;
}

/**
 * \brief This fu
 *
 * An Individual is a sparse array. When the Sudoku is loaded from a file,
 * some of its tiles already contain numbers ("constraints"). The
 * genetic algorithm has to guess the correct combination for the "empty" tiles
 * (filled with zeroes).
 *
 * The individuals used by the genetic algorithm do not represent the complete
 * grid, only the empty tiles, with values between 0 and dim_size-1 (n²-1).
 *
 * This function rebuilds the complete grid for one individual by adding the
 * values of the individual where they should be, in between the "static"
 * constraints of the Sudoku.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int* _evaluate_merge_problem_solution(unsigned int* individual,
                                               const Sudoku* sudoku)
 * \param individual a pointer to the genes of an Individual.
 * \param sudoku a pointer to the Sudoku to solve.
 * \return The merged grid or NULL if something goes wrong.
 * \sa _evaluate_merge_problem_solution
 */
static unsigned int* _evaluate_merge_problem_solution(unsigned int* individual,
                                                      const Sudoku* sudoku) {
  if (!individual) {
    return NULL;
  }

  if (!sudoku) {
    return NULL;
  }

  /* A sudoku is a grid make of n*n squares of n*n tiles (n² tiles per square).
   * dim_size = n² (number of tiles in a square, ex : 9 for a regular sudoku).
   */
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  // The size of one square.
  size_t size = dim_size * dim_size;

  /* Since an Individual is smaller than a complete grid (because it only holds
   * values for empty tiles), we need to use different indices.
   */
  unsigned int individual_index = 0;

  unsigned int* merge = ga_malloc(sizeof(unsigned int) * size);

  if (merge) {
    for (unsigned int i = 0; i < size; i++) {
      /* If the tile in the sudoku grid (the problem loaded from the file)
       * contains a number that we need to check against :
       */
      if (sudoku->problem[i] != 0) {
        merge[i] = (unsigned int)sudoku->problem[i];
      } else {
        /* Because the GA library works with number starting at 0, we add one
         * so that we work between 1 and n² and not 0 and n² - 1.
         */
        merge[i] = individual[individual_index] + 1;
        // We increment the index of the individual
        individual_index++;
      }
    }
    return merge;
  } else {
    return NULL;
  }
}

static unsigned int _evaluate_used_numbers_duplicates(unsigned int* used_values,
                                                      unsigned int used_size) {
  assert(used_values);
  assert(used_size);
  assert(used_size > 0);

  /* We create and initialize an array with the same size as "used_values"
   * "used_size" is a maximum size. The real size is "unique_size" declared
   * below.
   *
   * This array holds unique values from "used_values".
   */
  unsigned int unique_values[used_size];
  // This is the temporary size of "unique_values"
  unsigned int unique_size = 0;
  unsigned int duplicates = 0;

  // We loop over "used_values"
  for (unsigned int i = 0; i < used_size; i++) {
    bool no_duplicates_found = true;
    unsigned int j = 0;
    /* We loop over "unique_values" searching if the current number in
     *"used_values" is already in "unique_values". If that's the case, we can
     * increment the "duplicates" counter.
     */
    while (j < unique_size && no_duplicates_found) {
      if (unique_values[j] == used_values[i]) {
        // This boolean indicates that we've just found a duplicate element
        no_duplicates_found = false;
      } else {
        // The postfix increment operator increments the value after
        unique_values[unique_size++] = used_values[i];
      }
      j++;
    }

    // If we found duplicates
    if (!no_duplicates_found) {
      duplicates++;
    }
  }
  return duplicates;
}

static unsigned int _evaluate_check_row(unsigned int* merge,
                                        const Sudoku* sudoku,
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

static bool _evaluate_check_column(unsigned int* merge, const Sudoku* sudoku,
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
