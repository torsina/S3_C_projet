#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/ga/includes/ga.h"

int main(void) {
  ga_init();
  {
    GeneticGenerator* generator = genetic_generator_create(10);
    Population* population = ga_population_create(generator,10);
    Population* res = ga_population_destroy(population);
    ga_free(res);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
