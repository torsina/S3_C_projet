#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/ga/includes/ga.h"

int main(void) {
  ga_init();
  {
    const unsigned int NB_CHROMOSOMES = 10;
    // Creating a new GeneticGenerator with 10 chromosome cardinalities
    GeneticGenerator* generator = genetic_generator_create(NB_CHROMOSOMES);
    assert(generator);
    assert(genetic_generator_get_size(generator) == NB_CHROMOSOMES);
    // By default, all the cardinalities should be set to 0
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      assert(genetic_generator_get_cardinality(generator, i) == 0);
    }
    // Creating a population with this GeneticGenerator
    Population* population = ga_population_create(generator, 5);
    assert(population);
    Population* res = ga_population_destroy(population);
    assert(res == NULL);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
