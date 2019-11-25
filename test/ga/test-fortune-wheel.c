#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

#include "../../src/ga/includes/ga.h"

#include "../../src/ga/ga.inc"

static double random_double(double max) {
  return (double)rand()/(double)(RAND_MAX/max);
}

static Individual **_array_list_add(Individual **array, unsigned int *size,
                                    Individual *val) {
  Individual **new_array = realloc(array, (*size + 1) * sizeof(Individual *));
  if (new_array) {
    new_array[*size] = val;
    (*size)++;
    return new_array;
  } else {
    *size = 0;
    return NULL;
  }
}

FortuneWheel *fortune_wheel(Population *population,
                            unsigned int (*evaluate)(Individual *,
                                                     const void *)) {
  unsigned int scores_double[population->size];
  unsigned int total = 0;
  for (unsigned int i = 0; i < ga_population_get_size(population); i++) {
    // remplacer par _ga_individual_get_gene
    unsigned int score_int = evaluate(population->individuals[i], population);
    total += score_int;
    scores_double[i] = score_int;
    printf("%u", score_int);
    if (i != ga_population_get_size(population) - 1) {
      printf(", ");
    }
  }

  printf("\n");

  Individual **wheel = NULL;
  unsigned int size = 0;
  for (unsigned int i = 0; i < ga_population_get_size(population); i++) {
    double prob = scores_double[i] / total * ga_population_get_size(population);
    printf("[%u] -> %g", i, prob);
    if (i != ga_population_get_size(population) - 1) {
      printf(", ");
    }

    unsigned int whole_part = (unsigned int)prob;
    double fractional_part = prob - whole_part;

    for (unsigned int j = 0; j < whole_part; j++) {
      wheel = _array_list_add(wheel, &size, population->individuals[i]);
    }
    if(random_double(1.0) < fractional_part) {
      wheel = _array_list_add(wheel, &size, population->individuals[i]);
    }
  }
  FortuneWheel *fortune_wheel = malloc(sizeof *fortune_wheel);
  fortune_wheel->size = size;
  fortune_wheel->individuals = wheel;
}

int main(void) {
  ga_init();
  {}
  ga_finish();
  return EXIT_SUCCESS;
}
