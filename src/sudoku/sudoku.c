#include "includes/sudoku.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ga.h>
#include <yaml.h>

#include "sudoku.inc"

unsigned int sudoku_get_dim_size(const Sudoku *sudoku) {
  assert(sudoku);
  return sudoku->dim_size;
}

Sudoku *sudoku_create(const unsigned int dim_size) {
  Sudoku *sudoku = ga_malloc(sizeof(*sudoku));
  if (sudoku) {
    sudoku->dim_size = dim_size;

    if (dim_size) {
      unsigned int total_size = dim_size * dim_size;

      unsigned int *grid = ga_malloc(sizeof(*grid) * total_size);
      if (grid) {
        memset(grid, 0, sizeof(*grid) * total_size);
        sudoku->problem = grid;
        return sudoku;
      } else {
        ga_free(sudoku);
        return NULL;
      }
    } else {
      sudoku->problem = NULL;
      return sudoku;
    }
  } else {
    return NULL;
  }
}

void sudoku_destroy(Sudoku *sudoku) {
  if (sudoku) {
    ga_free(sudoku->problem);
    ga_free(sudoku);
  }
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
static unsigned int *_evaluate_merge_problem_solution(unsigned int *individual,
                                                      const Sudoku *sudoku) {
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

  unsigned int *merge = ga_malloc(sizeof(unsigned int) * size);

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

static unsigned int _evaluate_used_numbers_duplicates(unsigned int *used_values,
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

static unsigned int _evaluate_check_row(unsigned int *merge,
                                        const Sudoku *sudoku,
                                        unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int rowIndex = index / dim_size;
  unsigned int start = rowIndex * dim_size;
  unsigned int end = rowIndex * (dim_size + 1);
  // max int for a value = dim_size - 1
  unsigned int *used_values = ga_malloc(sizeof(unsigned int) * (dim_size - 1));
  unsigned int used_size = 0;
  for (unsigned int i = start; i < end; i++) {
    used_values[used_size] = merge[i];
    used_size++;
  }
  unsigned int duplicates =
      _evaluate_used_numbers_duplicates(used_values, used_size);
  ga_free(used_values);
  return duplicates;
}

static bool _evaluate_check_column(unsigned int *merge, const Sudoku *sudoku,
                                   unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int columnIndex = index % dim_size;
  // max int for a value = dim_size - 1
  unsigned int *used_values = ga_malloc(sizeof(unsigned int) * (dim_size - 1));
  unsigned int used_size = 0;
  for (unsigned int i = columnIndex; i < pow(dim_size, 2); i += dim_size) {
    used_values[used_size] = merge[i];
    used_size++;
  }
  unsigned int duplicates =
      _evaluate_used_numbers_duplicates(used_values, used_size);
  ga_free(used_values);
  return duplicates;
}

unsigned int evaluate(unsigned int *individual, const void *sudoku) {
  assert(individual);
  assert(sudoku);

  sudoku = (Sudoku *)sudoku;
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int score = pow(dim_size, 2);
  unsigned int *merge = _evaluate_merge_problem_solution(individual, sudoku);
  unsigned int duplicates = 0;
  // rows
  for (unsigned int i = 0; i < pow(dim_size, 2); i += dim_size) {
    duplicates += _evaluate_check_row(merge, sudoku, i);
  }
  // columns
  for (unsigned int i = 0; i < dim_size; i++) {
    duplicates += _evaluate_check_column(merge, sudoku, i);
  }
  ga_free(merge);
  // afraid that score could be negative if too much duplicates(because an error
  // can be counted multiple times) so squaring score to be sure
  return score * score - duplicates;
}

Sudoku *fill_sudoku(Sudoku *sudoku, FILE *file) {
  yaml_parser_t *parser = ga_malloc(sizeof(yaml_parser_t));
  if (!yaml_parser_initialize(parser)) {
    assert(printf("failed to init yaml parser\n"));
    yaml_parser_delete(parser);
    return NULL;
  }
  if (!file) {
    assert(printf("failed to open file\n"));
    yaml_parser_delete(parser);
    return NULL;
  }
  yaml_parser_set_input_file(parser, file);
  yaml_document_t *document = ga_malloc(sizeof(yaml_document_t));
  if (!yaml_parser_load(parser, document)) {
    assert(printf("end of document reached\n"));
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }
  yaml_node_t *root = yaml_document_get_root_node(document);
  if (!root) {
    assert(printf("document is empty\n"));
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }
  // if root is not a sequence(array) return
  if (root->type != YAML_SEQUENCE_NODE) {
    assert(printf("root node is not a sequence\n"));
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  yaml_node_item_t *iterator;
  printf("entering first array\n");
  unsigned int sudoku_index = 0;
  for (iterator = root->data.sequence.items.start;
       iterator < root->data.sequence.items.top; iterator++) {
    yaml_node_t *base = yaml_document_get_node(document, *iterator);
    // if first node is not a sequence of size dim_size
    if (base->type != YAML_SEQUENCE_NODE) {
      assert(printf("first node is not a sequence\n"));
      yaml_document_delete(document);
      yaml_parser_delete(parser);
      return NULL;
    }
    if (base->data.sequence.items.top - base->data.sequence.items.start !=
        sudoku->dim_size) {
      assert(printf("first array not of size %u\n", sudoku->dim_size));
      yaml_document_delete(document);
      yaml_parser_delete(parser);
      return NULL;
    }

    yaml_node_t *secondNode;
    yaml_node_item_t *secondIterator;
    assert(printf("entering second array\n"));
    for (secondIterator = base->data.sequence.items.start;
         secondIterator < base->data.sequence.items.top; secondIterator++) {
      secondNode = yaml_document_get_node(document, *secondIterator);
      if (secondNode->type == YAML_SCALAR_NODE) {
        if (sudoku_index >= (pow(sudoku->dim_size, 2))) {
          assert(printf("sudoku index above dim_size^2\n"));
          yaml_document_delete(document);
          yaml_parser_delete(parser);
        }
        if (!strcmp((char *)secondNode->data.scalar.value, "null")) {
          sudoku->problem[sudoku_index] = 0;
          assert(printf("value: null\n"));
        } else {
          char *endPtr;
          unsigned int value =
              strtol((char *)secondNode->data.scalar.value, &endPtr, 10);
          assert(printf("value: %u\n", value));
          sudoku->problem[sudoku_index] = value;
        }
        sudoku_index++;
      } else {
        assert(printf("second array element is not scalar\n"));
        yaml_document_delete(document);
        yaml_parser_delete(parser);
        return NULL;
      }
    }
  }

  yaml_document_delete(document);
  yaml_parser_delete(parser);
  return sudoku;
}
