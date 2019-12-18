//
// Created by Aelis on 17/12/2019.
//

#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "../../src/sudoku/includes/sudoku.h"
#include "../../src/sudoku/sudoku.inc"

bool problem_equal(unsigned int* first, unsigned int* second,
                   unsigned int size) {
  return !memcmp(first, second, size);
}

typedef struct {
  char* path;
  unsigned int* values;
  unsigned int dim_size;
} Test;

typedef struct {
  Test** tests;
  unsigned int size;
} Tests;

Test* create_test(char* path, unsigned int* values, unsigned int dim_size) {
  Test* test = malloc(sizeof(Test));
  unsigned int size = dim_size * dim_size;
  char str[80] = "../../examples/";
  strcat(str, path); // NOLINT
  strcat(str, ".yaml"); // NOLINT
  test->path = malloc((strlen(str) * sizeof(char)) + 1);
  strcpy(test->path, str); // NOLINT
  test->values = malloc(sizeof(unsigned int) * size);
  memcpy(test->values, values, size);
  test->dim_size = dim_size;
  return test;
}

Tests* create_tests(unsigned int size) {
  Tests* tests = malloc(sizeof(Tests));
  tests->tests = malloc(sizeof(Test) * size);
  tests->size = 0;
}

void destroy_tests(Tests* tests) {
  for (unsigned int i = 0; i < tests->size; i++) {
    Test* test = tests->tests[i];
    free(test->path);
    free(test->values);
    free(tests->tests[i]);
  }
  free(tests);
}

Tests* add_test(Tests* tests, Test* test) {
  tests->tests[tests->size] = test;
  tests->size++;
  return tests;
}

int main(void) {
  unsigned int NUMBER_OF_TESTS = 5;
  Tests* tests = create_tests(NUMBER_OF_TESTS);
  // create tests
  add_test(tests, create_test("three_by_three",
                              (unsigned int[]){2, 0, 0, 0, 0, 0, 0, 1, 0}, 3));
  add_test(tests, create_test("null", (unsigned int[]){0, 0, 0, 0}, 2));
  add_test(tests,
           create_test("irregular_size_1", (unsigned int[]){1, 2, 1, 2, 3}, 2));
  add_test(tests, create_test("irregular_size_2",
                              (unsigned int[]){1, 2, 3, 4, 0, 5}, 2));
  add_test(tests,
           create_test("full", (unsigned int[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 3));

  for (unsigned int i = 0; i < tests->size; i++) {
    printf("--------START------\n");
    Test* test = tests->tests[i];

    FILE* file;
    printf("path: %s\n", test->path);
    file = fopen(test->path, "r");
    assert(file != NULL);
    Sudoku* sudoku = sudoku_create(test->dim_size);
    sudoku = fill_sudoku(sudoku, file);
    if (strstr(test->path, "irregular_size_1") ||
        strstr(test->path, "irregular_size_2")) {
      assert(sudoku == NULL);
    } else if (strstr(test->path, "three_by_three") ||
               strstr(test->path, "null") || strstr(test->path, "full")) {
      assert(problem_equal(sudoku->problem, test->values,
                           test->dim_size * test->dim_size));
    } else {
      assert(printf("invalid case: %s\n", test->path));
    }
    fclose(file);
    sudoku_destroy(sudoku);
    printf("--------END--------\n");
  }
  destroy_tests(tests);
  return EXIT_SUCCESS;
}
