#include "includes/sudoku.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ga.h>
#include <yaml.h>

#include "sudoku.inc"

#define PRINT_VERBOSE(verbose, str) ((verbose) ? printf("%s", str) : (void)0);

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
        sudoku->dim_size = 0;
        sudoku->problem = NULL;
        return sudoku;
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

unsigned int *evaluate_merge_problem_solution(unsigned int *individual,
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

  unsigned int duplicates = 0;

  for (unsigned int i = 0; i < used_size; i++) {
    for (unsigned int j = i + 1; j < used_size; j++) {
      /* If duplicate found then increment count by 1 */
      if (used_values[i] == used_values[j]) {
        duplicates++;
        break;
      }
    }
  }
  return duplicates;
}

static unsigned int _evaluate_check_row(unsigned int *merge,
                                        const Sudoku *sudoku,
                                        unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  // Tile index / number of lines
  unsigned int rowIndex = index / dim_size;
  // Offset from the beginning of the array
  unsigned int start = rowIndex * dim_size;
  // unsigned int end = rowIndex * (dim_size + 1);
  unsigned int end = start + dim_size - 1;
  // max int for a value = dim_size - 1
  unsigned int *used_values = ga_malloc(sizeof(unsigned int) * (dim_size));
  // unsigned int *used_values = ga_malloc(sizeof(unsigned int) * (dim_size -
  // 1));
  unsigned int used_size = 0;
  for (unsigned int i = start; i <= end; i++) {
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
  unsigned int *used_values = ga_malloc(sizeof(unsigned int) * (dim_size));
  unsigned int used_size = 0;
  for (unsigned int i = columnIndex; i < pow(dim_size, 2); i += dim_size) {
    used_values[used_size] = merge[i];
    used_size++;
  }
  // -1 because the last loop increments
  unsigned int duplicates =
      _evaluate_used_numbers_duplicates(used_values, used_size - 1);
  ga_free(used_values);
  return duplicates;
}

static bool _evaluate_check_box(unsigned int *merge, const Sudoku *sudoku,
                                unsigned int index) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);

  unsigned int box_length = (unsigned int)sqrt(dim_size);

  unsigned int columnIndex = index % dim_size;
  assert(columnIndex + box_length <= dim_size);
  // max int for a value = dim_size - 1
  unsigned int *used_values = ga_malloc(sizeof(unsigned int) * (dim_size));
  unsigned int used_size = 0;

  // Iterate over the columns of the box
  for (unsigned int i = columnIndex;
       i < dim_size && i < columnIndex + box_length; i++) {
    for (unsigned int j = 0; j < box_length; j++) {
      used_values[used_size] = merge[i + j * dim_size];
      used_size++;
    }
  }
  // -1 because the last loop increments
  unsigned int duplicates =
      _evaluate_used_numbers_duplicates(used_values, used_size - 1);
  ga_free(used_values);
  return duplicates;
}

unsigned int _evaluate_similarity(unsigned int *merge, const Sudoku *sudoku) {
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int score = 0;
  for (unsigned int i = 0; i < dim_size * dim_size; i++) {
    score += abs((int)merge[i] - (int)sudoku->problem[i]);
  }
  return score;
}

unsigned int _potential_max_score(const Sudoku *sudoku) {
  return (unsigned int)pow(sudoku->dim_size, 2) *
         3 /* + (sudoku->dim_size - 1) * sudoku->dim_size * sudoku->dim_size*/;
}

unsigned int evaluate(unsigned int *individual, const void *sudoku) {
  assert(individual);
  assert(sudoku);

  sudoku = (Sudoku *)sudoku;
  unsigned int dim_size = sudoku_get_dim_size(sudoku);
  unsigned int box_length = (unsigned int)sqrt(dim_size);
  // unsigned int score = (unsigned int)pow(dim_size, 2) * 3;
  unsigned int score = _potential_max_score(sudoku);
  unsigned int *merge = evaluate_merge_problem_solution(individual, sudoku);
  unsigned int duplicates = 0;
  unsigned int test = 0;
  // rows
  for (unsigned int i = 0; i < dim_size * dim_size; i += dim_size) {
    duplicates += _evaluate_check_row(merge, sudoku, i);
    test += dim_size - _evaluate_check_row(merge, sudoku, i);
  }
  // columns
  for (unsigned int i = 0; i < dim_size; i++) {
    duplicates += _evaluate_check_column(merge, sudoku, i);
    test += dim_size - _evaluate_check_column(merge, sudoku, i);
  }
  // boxes
  for (unsigned int i = 0; i < box_length; i++) {
    for (unsigned int j = 0; j < box_length; j++) {
      // i = horizontal box position
      // j = horizontal box position
      // The index points at the top left corner of the box
      unsigned int index = i * box_length + j * box_length * dim_size;
      duplicates += _evaluate_check_box(merge, sudoku, index);
      test += dim_size - _evaluate_check_box(merge, sudoku, index);
    }
  }

  // duplicates += _evaluate_similarity(merge, sudoku);
  ga_free(merge);

  // return test;
  // 2 is just a weight here
  if (duplicates > score) {
    return 0;
  }
  return score - duplicates;
}

bool is_valid(unsigned int *solution, const Sudoku *sudoku) {
  if (!solution || !sudoku) {
    return false;
  } else {
    unsigned int dim_size = sudoku_get_dim_size(sudoku);
    return is_max_score(evaluate(solution, sudoku), sudoku);
  }
}

bool is_max_score(unsigned int score, const Sudoku *sudoku) {
  if (!sudoku) {
    return false;
  } else {
    unsigned int max_score = _potential_max_score(sudoku);
    return score >= max_score;
  }
}

Sudoku *fill_sudoku(Sudoku *sudoku, FILE *file) {
  if (!file) {
    assert(printf("Failed to open file.\n"));
    return NULL;
  }

  if (!sudoku) {
    return NULL;
  }

  yaml_parser_t _parser;
  // yaml_parser_t *parser = ga_malloc(sizeof(yaml_parser_t));
  yaml_parser_t *parser = &_parser;

  if (!parser) {
    return NULL;
  }

  if (!yaml_parser_initialize(parser)) {
    assert(printf("Failed to init YAML parser.\n"));
    yaml_parser_delete(parser);
    return NULL;
  }
  /*if (!file) {
    assert(printf("failed to open file\n"));
    yaml_parser_delete(parser);
    return NULL;
  }*/
  // Binding the YAML parser to the file.
  yaml_parser_set_input_file(parser, file);
  // Creating a YAML document for reading
  // yaml_document_t *document = ga_malloc(sizeof(yaml_document_t));

  yaml_document_t _doc;
  yaml_document_t *document = &_doc;
  if (!document) {
    assert(printf("The document could not be created.\n"));
    yaml_parser_delete(parser);
    return NULL;
  }

  if (!yaml_parser_load(parser, document)) {
    assert(printf("End of document reached.\n"));
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  yaml_node_t *root = yaml_document_get_root_node(document);
  if (!root) {
    assert(printf("Document is empty.\n"));
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  // If root is not a sequence(array) return
  if (root->type != YAML_SEQUENCE_NODE) {
    assert(printf("Root node is not a sequence.\n"));
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  yaml_node_item_t *iterator;
  assert(printf("Entering first array.\n"));
  unsigned int sudoku_index = 0;

  // Iterating over the root node's children (every line)
  for (iterator = root->data.sequence.items.start;
       iterator < root->data.sequence.items.top; iterator++) {
    yaml_node_t *base = yaml_document_get_node(document, *iterator);

    // If the node is not a sequence(array) return
    if (base->type != YAML_SEQUENCE_NODE) {
      assert(printf("First node is not a sequence.\n"));
      yaml_document_delete(document);
      yaml_parser_delete(parser);
      return NULL;
    }

    // If first node is not a sequence of size dim_size
    if (base->data.sequence.items.top - base->data.sequence.items.start !=
        sudoku->dim_size) {
      assert(printf("First array not of size %u.\n", sudoku->dim_size));
      yaml_document_delete(document);
      yaml_parser_delete(parser);
      return NULL;
    }

    // Temp variable for storing the current scalar node
    yaml_node_t *second_node;
    yaml_node_item_t *second_iterator;
    assert(printf("Entering second array.\n"));
    // This iterator is used for iterating over the child nodes (a line).
    for (second_iterator = base->data.sequence.items.start;
         second_iterator < base->data.sequence.items.top; second_iterator++) {
      // We get the current node in the line.
      second_node = yaml_document_get_node(document, *second_iterator);
      // The node must be a scalar node
      if (second_node->type == YAML_SCALAR_NODE) {
        /* We make sure that the index is not too big (that the line is not too
         long).*/
        if (sudoku_index >= (sudoku->dim_size * sudoku->dim_size)) {
          assert(printf("Sudoku index above dim_size^2.\n"));
          yaml_document_delete(document);
          yaml_parser_delete(parser);
          return NULL;
        }

        // If the node contains the literal text "null"
        if (!strcmp((char *)second_node->data.scalar.value, "null")) {
          // We insert the value 0 at the index
          sudoku->problem[sudoku_index] = 0;
          assert(printf("value: null\n"));
        } else {
          char *end_ptr;
          // We convert the value to an integer value
          unsigned int value =
              strtol((char *)second_node->data.scalar.value, &end_ptr, 10);
          if (end_ptr &&
              strcmp((char *)second_node->data.scalar.value, end_ptr) == 0) {
            assert(printf("Invalid integer value %s.\n",
                          (char *)second_node->data.scalar.value));
            yaml_document_delete(document);
            yaml_parser_delete(parser);
            return NULL;
          }
          assert(printf("Value: %u.\n", value));
          sudoku->problem[sudoku_index] = value;
        }
        // We increment the index in the Sudoku struct.
        sudoku_index++;
      } else {
        assert(printf("Second array element is not scalar.\n"));
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

static unsigned int **_array_list_add(unsigned int **array, unsigned int *size,
                                      const unsigned int *val) {
  unsigned int **new_array = realloc(array, (*size + 1) * sizeof(*new_array));
  if (new_array) {
    new_array[*size] = (unsigned int *)val;
    (*size)++;
    return new_array;
  } else {
    *size = 0;
    return NULL;
  }
}

Sudoku *read_sudoku(FILE *file, bool verbose) {
  if (!file) {
    PRINT_VERBOSE(verbose, "Failed to open file.\n");
    return NULL;
  }

  // Holds the lines, in a linear layout
  unsigned int *problem = NULL;
  // Number of values per line.
  unsigned int dim_size = 0;

  yaml_parser_t _parser;
  yaml_parser_t *parser = &_parser;

  if (!parser) {
    return NULL;
  }

  if (!yaml_parser_initialize(parser)) {
    PRINT_VERBOSE(verbose, "Failed to init YAML parser.\n");
    yaml_parser_delete(parser);
    return NULL;
  }

  // Binding the YAML parser to the file.
  yaml_parser_set_input_file(parser, file);

  // Creating a YAML document for reading
  yaml_document_t _doc;
  yaml_document_t *document = &_doc;
  if (!document) {
    PRINT_VERBOSE(verbose, "The document could not be created.\n");
    yaml_parser_delete(parser);
    return NULL;
  }

  if (!yaml_parser_load(parser, document)) {
    PRINT_VERBOSE(verbose, "End of document reached.\n");
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  yaml_node_t *root = yaml_document_get_root_node(document);
  if (!root) {
    PRINT_VERBOSE(verbose, "Document is empty.\n");
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  // If root is not a sequence(array) return
  if (root->type != YAML_SEQUENCE_NODE) {
    PRINT_VERBOSE(verbose, "Root node is not a sequence.\n");
    yaml_document_delete(document);
    yaml_parser_delete(parser);
    return NULL;
  }

  yaml_node_item_t *iterator;
  PRINT_VERBOSE(verbose, "Entering first array.\n");
  unsigned int sudoku_index = 0;

  dim_size = root->data.sequence.items.top - root->data.sequence.items.start;

  // Array initialization
  problem = calloc(dim_size * dim_size, sizeof(*problem));

  if (verbose) {
    printf("Found dim_size : %u\n", dim_size);
  }

  // Iterating over the root node's children (every line)
  for (iterator = root->data.sequence.items.start;
       iterator < root->data.sequence.items.top; iterator++) {
    yaml_node_t *base = yaml_document_get_node(document, *iterator);

    // If the node is not a sequence(array) return
    if (base->type != YAML_SEQUENCE_NODE) {
      PRINT_VERBOSE(verbose, "First node is not a sequence.\n");
      yaml_document_delete(document);
      yaml_parser_delete(parser);
      ga_free(problem);
      return NULL;
    }

    // Length of the line
    unsigned int node_line_length =
        base->data.sequence.items.top - base->data.sequence.items.start;

    // If first node is not a sequence of size dim_size
    if (node_line_length != dim_size) {
      if (verbose) {
        printf("First array not of size %u.\n", dim_size);
      }
      yaml_document_delete(document);
      yaml_parser_delete(parser);
      ga_free(problem);
      return NULL;
    }

    // Temp variable for storing the current scalar node
    yaml_node_t *second_node;
    yaml_node_item_t *second_iterator;
    PRINT_VERBOSE(verbose, "Entering second array.\n");
    // This iterator is used for iterating over the child nodes (a line).
    for (second_iterator = base->data.sequence.items.start;
         second_iterator < base->data.sequence.items.top; second_iterator++) {
      // We get the current node in the line.
      second_node = yaml_document_get_node(document, *second_iterator);
      // The node must be a scalar node
      if (second_node->type == YAML_SCALAR_NODE) {
        /* We make sure that the index is not too big (that the line is not too
         long).*/
        if (sudoku_index >= (dim_size * dim_size)) {
          PRINT_VERBOSE(verbose, "Sudoku index above dim_size^2.\n");
          yaml_document_delete(document);
          yaml_parser_delete(parser);
          ga_free(problem);
          return NULL;
        }

        // If the node contains the literal text "null"
        if (!strcmp((char *)second_node->data.scalar.value, "null")) {
          // We insert the value 0 at the index
          problem[sudoku_index] = 0;
          // sudoku->problem[sudoku_index] = 0;
          PRINT_VERBOSE(verbose, "Value : null.\n");
        } else {
          char *end_ptr;
          // We convert the value to an integer value
          unsigned int value =
              strtol((char *)second_node->data.scalar.value, &end_ptr, 10);
          if (end_ptr &&
              strcmp((char *)second_node->data.scalar.value, end_ptr) == 0) {
            if (verbose) {
              printf("Invalid integer value %s.\n",
                     (char *)second_node->data.scalar.value);
            }

            yaml_document_delete(document);
            yaml_parser_delete(parser);
            ga_free(problem);
            return NULL;
          }
          if (verbose) {
            printf("Value: %u.\n", value);
          }

          if (value > dim_size) {
            if (verbose) {
              printf("Integer value too big (%u > %u).\n", value, dim_size);
            }
            yaml_document_delete(document);
            yaml_parser_delete(parser);
            ga_free(problem);
            return NULL;
          }

          problem[sudoku_index] = value;
        }
        // We increment the index in the Sudoku struct.
        sudoku_index++;
      } else {
        PRINT_VERBOSE(verbose, "Second array element is not scalar.\n");
        yaml_document_delete(document);
        yaml_parser_delete(parser);
        ga_free(problem);
        return NULL;
      }
    }
  }

  // This sudoku is empty for now
  Sudoku *sudoku = sudoku_create(0);
  sudoku->problem = problem;
  sudoku->dim_size = dim_size;

  yaml_document_delete(document);
  yaml_parser_delete(parser);
  return sudoku;
}

unsigned int sudoku_empty_tiles(const Sudoku *sudoku) {
  assert(sudoku);
  assert(sudoku->problem);
  unsigned int total_size = sudoku->dim_size * sudoku->dim_size;
  unsigned int counter = 0;
  for (unsigned int i = 0; i < total_size; i++) {
    // If the value is 0
    if (!sudoku->problem[i]) {
      counter++;
    }
  }
  return counter;
}

void sudoku_print(unsigned int *solution, Sudoku *sudoku) {
  if (solution && sudoku) {
    unsigned int *array = evaluate_merge_problem_solution(solution, sudoku);

    if (!array) {
      return;
    }

    for (unsigned int x = 0; x < sudoku->dim_size; x++) {
      for (unsigned int y = 0; y < sudoku->dim_size; y++) {
        size_t index = x * sudoku->dim_size + y;
        printf("%u ", array[index]);
      }
      printf("\n");
    }
    free(array);
  }
}

bool save_sudoku(const Sudoku *sudoku, FILE *file) {
  if (!file) {
    assert(printf("No file provided"));
    return false;
  }
  if (!sudoku || !sudoku->dim_size || !sudoku->problem) {
    assert(printf("No sudoku provided"));
    return false;
  }
  for (unsigned int x = 0; x < sudoku->dim_size; x++) {
    fprintf(file, "- [");
    for (unsigned int y = 0; y < sudoku->dim_size; y++) {
      size_t index = x * sudoku->dim_size + y;
      if (y == sudoku->dim_size - 1) {
        fprintf(file, " %u", sudoku->problem[index]);
      } else {
        fprintf(file, " %u,", sudoku->problem[index]);
      }
    }
    fprintf(file, "]\n");
  }
}
