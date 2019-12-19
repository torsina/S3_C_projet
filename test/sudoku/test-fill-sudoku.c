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
  unsigned int dim_size;
  unsigned int* values;
} Test;

typedef struct {
  Test** tests;
  unsigned int index;
  unsigned int size;
} Tests;

Test* create_test(char* path, unsigned int* values, unsigned int dim_size) {
  Test* test = malloc(sizeof(Test));
  assert(test);
  unsigned int size = dim_size * dim_size;
  // size > 0
  assert(size);
  assert(printf("Creating a test (%s) with size %u.\n", path, size));
  char str[80] = "../../examples/";

  strncat(str, path, strlen(path));
  strncat(str, ".yaml", 6);
  test->path = calloc(strlen(str) + 1, sizeof(char));
  assert(test->path);
  strncpy(test->path, str, strlen(str));
  unsigned int* array = calloc(size, sizeof(unsigned int));
  assert(array);
  test->values = array;
  assert(test->values);
  memcpy(test->values, values, size);
  test->dim_size = dim_size;
  return test;
}

Tests* create_tests(unsigned int size) {
  Tests* tests = malloc(sizeof(Tests));
  assert(tests);
  tests->tests = calloc(size, sizeof(Test));
  assert(tests->tests);
  tests->index = 0;
  tests->size = size;
  return tests;
}

void destroy_tests(Tests* tests) {
  for (unsigned int i = 0; i < tests->size; i++) {
    Test* test = tests->tests[i];
    free(test->path);
    free(test->values);
    free(tests->tests[i]);
  }
  free(tests->tests);
  free(tests);
}

Tests* add_test(Tests* tests, Test* test) {
  if (tests->index < tests->size) {
    tests->tests[tests->index++] = test;
    return tests;
  } else {
    return NULL;
  }
}

int main(void) {
  unsigned int NUMBER_OF_TESTS = 5;
  Tests* tests = create_tests(NUMBER_OF_TESTS);
  assert(tests);
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
    printf("--------START[%u]------\n", i);
    Test* test = tests->tests[i];

    FILE* file;
    printf("Path: %s\n", test->path);
    file = fopen(test->path, "r");
    assert(file);
    assert(printf("Creating sudoku for %s.\n", test->path));
    Sudoku* sudoku = sudoku_create(test->dim_size);
    assert(sudoku);
    Sudoku *temp = fill_sudoku(sudoku, file);
    if (strstr(test->path, "irregular_size_1") ||
        strstr(test->path, "irregular_size_2")) {
      sudoku_destroy(sudoku);
      sudoku = NULL;
      assert(temp == NULL);
    } else if (strstr(test->path, "three_by_three") ||
               strstr(test->path, "null") || strstr(test->path, "full")) {
      assert(problem_equal(temp->problem, test->values,
                           test->dim_size * test->dim_size));
      sudoku_destroy(sudoku);
      sudoku = NULL;
    } else {
      assert(printf("Invalid case: %s\n", test->path));
      sudoku_destroy(sudoku);
      sudoku = NULL;
    }
    fclose(file);
    sudoku_destroy(sudoku);
    printf("--------END--------\n");
  }
  destroy_tests(tests);
  return EXIT_SUCCESS;
}
