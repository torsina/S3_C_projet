#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <ga.h>
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

static const char *const ERROR_INVALID_SUDOKU =
    "[ERROR] The sudoku is invalid.\n";

static const char *const ERROR_EMPTY_SUDOKU = "[ERROR] The sudoku is empty.\n";

static const char *const ERROR_GENERATOR =
    "[ERROR] The GeneticGenerator could not be created.\n";

static const char *const ERROR_GENERATOR_CARDINALITIES =
    "[ERROR] The GeneticGenerator's cardinalities could not be created.\n";

static const char *const ERROR_POPULATION =
    "[ERROR] The Population could not be created.\n";

static const char *const ERROR_POPULATION_EVOLVE =
    "[ERROR] The Population could not evolve.\n";

static const char *const ERROR_BEST_INDIVIDUAL =
    "[ERROR] Could not find the best individual.\n";

static const char *const HELP_MESSAGE =
    "Usages :\n"
    "1. sudoku --test : display this message\n"
    "2. sudoku <YAML file> <crossover> <mutation> <nb ind> <max it>\n"
    "\t|<YAML file> : path to the YAML file to load the sudoku from.\n"
    "\t|<crossover> : crossover probability. English notation (dot), [0, 1].\n"
    "\t|<mutation>  : mutation probability. English notation (dot), [0, 1].\n"
    "\t|<nb ind>    : number of individuals per population/generation. Positive"
    " integer >0.\n"
    "\t|<max it>    : max iterations count. The program will stop if the "
    "problem"
    "isn't solved and this number of iteration is reached. Positive integer.\n";

static const char *const HELP_PARAM = "--help";

static const char *const VERBOSE_PARAM = "--verbose";

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

static uint32_t read_count(const char **argv, size_t index) {
  char *end_ptr;
  int32_t count = strtol(argv[index], &end_ptr, 10);

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
  return (uint32_t)count;
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
  bool verbose = false;

  if (argc < 6) {
    if (argc > 1) {
      /* strcmp won't work because HELP_PARAM has a trailing \0.
       * Here, by using strncmp we only compare the characters before the \0.
       */
      if (strncmp(argv[1], HELP_PARAM, strlen(HELP_PARAM)) == 0) {
        printf("%s", HELP_MESSAGE);
        return EXIT_FAILURE;
      } else {
        fprintf(stderr, "%s", ERROR_MESSAGE);
        return EXIT_FAILURE;
      }
    } else {
      fprintf(stderr, "%s", ERROR_MESSAGE);
      return EXIT_FAILURE;
    }
  } else {
    const char *yaml_path = argv[1];

    // Opens the file in read-only mode
    FILE *yaml_file = fopen(yaml_path, "r");
    if (yaml_file) {
      double crossover_prob = read_probability(argv, 2);

      double mutation_prob = read_probability(argv, 3);

      uint32_t nb_ind = read_count(argv, 4);

      if (nb_ind <= 0) {
        fprintf(stderr, ERROR_ZERO_LONG, 4);
        return EXIT_FAILURE;
      }

      uint32_t nb_iter = read_count(argv, 5);

      if (argc >= 7 &&
          strncmp(argv[6], VERBOSE_PARAM, strlen(VERBOSE_PARAM)) == 0) {
        verbose = true;
        printf("Verbose mode is on.\n");
      }

      // LOAD YAML HERE
      Sudoku *sudoku = read_sudoku(yaml_file, verbose);
      if (!sudoku) {
        fprintf(stderr, "%s", ERROR_INVALID_SUDOKU);
        fclose(yaml_file);
        return EXIT_FAILURE;
      }

      // Displaying the Sudoku
      if (verbose) {
        printf("Loaded Sudoku grid :\n");
        for (unsigned int x = 0; x < sudoku->dim_size; x++) {
          for (unsigned int y = 0; y < sudoku->dim_size; y++) {
            size_t index = x * sudoku->dim_size + y;
            printf("%u ", sudoku->problem[index]);
          }
          printf("\n");
        }
      }

      ga_init();
      {
        // The number of empty tiles in the grid, size of 1 solution
        unsigned int nb_empty_tiles = sudoku_empty_tiles(sudoku);
        if (nb_empty_tiles == 0) {
          fprintf(stderr, "%s", ERROR_EMPTY_SUDOKU);
          fclose(yaml_file);
          sudoku_destroy(sudoku);
          return EXIT_FAILURE;
        }

        GeneticGenerator *generator = genetic_generator_create(nb_empty_tiles);
        if (!generator) {
          fprintf(stderr, "%s", ERROR_GENERATOR);
          fclose(yaml_file);
          sudoku_destroy(sudoku);
          return EXIT_FAILURE;
        }

        for (unsigned int i = 0; i < nb_empty_tiles; i++) {
          if (!genetic_generator_set_cardinality(generator, i,
                                                 sudoku->dim_size)) {
            fprintf(stderr, "%s", ERROR_GENERATOR_CARDINALITIES);
            fclose(yaml_file);
            sudoku_destroy(sudoku);
            genetic_generator_destroy(generator);
            return EXIT_FAILURE;
          }
        }

        Population *population = ga_population_create(generator, nb_ind);
        if (!population) {
          fprintf(stderr, "%s", ERROR_POPULATION);
          fclose(yaml_file);
          sudoku_destroy(sudoku);
          genetic_generator_destroy(generator);
          return EXIT_FAILURE;
        }

        // TODO(T-MMLR) : print the generation in verbose mode
        // if (verbose) {
        // }

        unsigned int best_score = 0;
        unsigned int *best = population_best_individual(population, evaluate,
                                                        sudoku, &best_score);
        if (!best) {
          fprintf(stderr, "%s", ERROR_BEST_INDIVIDUAL);
          fclose(yaml_file);
          sudoku_destroy(sudoku);
          genetic_generator_destroy(generator);
          ga_population_destroy(population);
          return EXIT_FAILURE;
        }

        // We already have the first generation
        unsigned int generation = 1;

        printf("Best solution for generation %u with score %u.\n", generation,
               best_score);
        sudoku_print(best, sudoku);
        printf("------------------------\n");
        if (is_max_score(best_score, sudoku)) {
          printf("Found the solution !");
        }

        while (generation < nb_iter) {
          if (verbose) {
            printf("\t--GENERATION %u--\n", generation);
          }

          Population *next_generation =
              ga_population_next(population, (const float)crossover_prob,
                                 (const float)mutation_prob, evaluate, sudoku);
          if (!next_generation) {
            fprintf(stderr, "%s", ERROR_POPULATION_EVOLVE);
            fclose(yaml_file);
            sudoku_destroy(sudoku);
            genetic_generator_destroy(generator);
            ga_population_destroy(population);
          }

          population = next_generation;
          generation++;

          best = population_best_individual(population, evaluate, sudoku,
                                            &best_score);
          // save best individual to solution.yaml
          if (generation == nb_iter) {
            FILE *save = fopen("./solution.yaml", "w");
            unsigned int *finished_sudoku =
                evaluate_merge_problem_solution(best, sudoku);
            Sudoku *sudoku_merged = sudoku_create(sudoku->dim_size);
            free(sudoku_merged->problem);
            sudoku_merged->problem = finished_sudoku;
            save_sudoku(sudoku_merged, save);
            fclose(save);
            sudoku_destroy(sudoku_merged);
          }

          if (!best) {
            fprintf(stderr, "%s", ERROR_BEST_INDIVIDUAL);
            fclose(yaml_file);
            sudoku_destroy(sudoku);
            genetic_generator_destroy(generator);
            ga_population_destroy(population);
            return EXIT_FAILURE;
          }

          printf("Best solution for generation %u with score %u.\n", generation,
                 best_score);
          sudoku_print(best, sudoku);
          printf("------------------------\n");
          if (is_max_score(best_score, sudoku)) {
            printf("Found the solution !");
          }
        }
        genetic_generator_destroy(generator);
        ga_population_destroy(population);
      }
      fclose(yaml_file);
      sudoku_destroy(sudoku);
      ga_finish();

    } else {
      fprintf(stderr, ERROR_FILE_NOT_FOUND);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
