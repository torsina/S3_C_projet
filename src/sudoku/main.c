#include <stdio.h>
#include <stdlib.h>

#include <yaml.h>

#include "includes/sudoku.h"

#include "sudoku.inc"

static const char *const ERROR_MESSAGE =
    "[ERROR] Wrong usage of the program, see --help.\n";

static const char *const ERROR_FILE_NOT_FOUND = "[ERROR] File not found.\n";

static const char *const ERROR_INVALID_DOUBLE =
    "[ERROR] Invalid double format at position %zu : %s.\n";

static const char *const ERROR_RANGE_DOUBLE =
    "[ERROR] Invalid double range at position %zu : %s. Must be between 0 and "
    "1.\n";

static const char *const ERROR_INVALID_LONG =
    "[ERROR] Invalid integer format at position %zu : %s.\n";

static const char *const ERROR_RANGE_LONG =
    "[ERROR] Invalid integer range at position %zu : %s. Must be positive.\n";

static const char *const ERROR_ZERO_LONG =
    "[ERROR] Integer at position %zu must be strictly positive.\n";

static const char *const HELP_MESSAGE =
    "Usages :\n"
    "1. sudoku --test : display this message\n"
    "2. sudoku <YAML file> <crossover> <mutation> <nb ind> <max it>\n"
    "\t|<YAML file> : path to the YAML file to load the sudoku from.\n"
    "\t|<crossover> : crossover probability. English notation (dot), [0, 1].\n"
    "\t|<mutation>  : mutation probability. English notation (dot), [0, 1].\n"
    "\t|<nb ind>    : number of individuals per population/generation. Positive"
    " integer.\n"
    "\t|<max it>    : max iterations count. The program will stop if the "
    "problem"
    "isn't solved and this number of iteration is reached. Positive integer.\n";

static const char *const HELP_PARAM = "--help";

static double read_probability(const char **argv, size_t index) {
  char *end_ptr;
  double prob = strtod(argv[index], &end_ptr);

  /* end_ptr can be NULL, in which case it indicates that the algorithm
    successfully read the double but no other character was present after.*/
  if (end_ptr && strcmp(argv[index], end_ptr) == 0) {
    fprintf(stderr, ERROR_INVALID_DOUBLE, index, argv[index]);
    exit(EXIT_FAILURE);
  }

  if (prob < 0.0 || prob > 1.0) {
    fprintf(stderr, ERROR_RANGE_DOUBLE, index, argv[index]);
    exit(EXIT_FAILURE);
  }
  return prob;
}

static unsigned long int read_count(const char **argv, size_t index) {
  char *end_ptr;
  long int count = strtol(argv[index], &end_ptr, 10);

  /* end_ptr can be NULL, in which case it indicates that the algorithm
    successfully read the long but no other character was present after.*/
  if (end_ptr && strcmp(argv[index], end_ptr) == 0) {
    fprintf(stderr, ERROR_INVALID_LONG, index, argv[index]);
    exit(EXIT_FAILURE);
  }

  if (count < 0) {
    fprintf(stderr, ERROR_RANGE_LONG, index, argv[index]);
    exit(EXIT_FAILURE);
  }
  return (unsigned long int)count;
}

int main(int argc, const char **argv) {
  /*
   * argv :
   * - Name of the program
   * - Path to the YAML file to load the Sudoku from
   * - Crossover probability
   * - Mutation probability
   * - Number of individuals in the Population
   * - Maximum number of iterations
   */
  if (argc < 6) {
    if (argc > 1) {
      /* strcmp won't work because HELP_PARAM has a trailing \0.
       * Here, by using strncmp we only compare the characters before the \0.
       */
      if (strncmp(argv[1], HELP_PARAM, strlen(HELP_PARAM)) == 0) {
        printf(HELP_MESSAGE);
        return EXIT_FAILURE;
      } else {
        fprintf(stderr, ERROR_MESSAGE);
        return EXIT_FAILURE;
      }
    } else {
      fprintf(stderr, ERROR_MESSAGE);
      return EXIT_FAILURE;
    }
  } else {
    const char *yaml_path = argv[1];

    // Opens the file in read-only mode
    FILE *yaml_file = fopen(yaml_path, "r");
    if (yaml_file) {
      double crossover_prob = read_probability(argv, 2);

      double mutation_prob = read_probability(argv, 3);

      unsigned long int nb_ind = read_count(argv, 4);

      if (nb_ind <= 0) {
        fprintf(stderr, ERROR_ZERO_LONG, 4);
        return EXIT_FAILURE;
      }

      unsigned long int nb_iter = read_count(argv, 5);

      // LOAD YAML HERE

    } else {
      fprintf(stderr, ERROR_FILE_NOT_FOUND);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
