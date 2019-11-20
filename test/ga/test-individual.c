#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/ga/includes/ga.h"

int main(void) {
  GeneticGenerator* generator = genetic_generator_create(20);
  Individual* foo = genetic_generator_individual(generator);
  for(unsigned int i = 0; i < 20; i++) {
    genetic_generator_set_cardinality(generator, i, i*20);
    unsigned int value = ga_individual_get_gene(foo, i);
    assert(value >= 0 &&
               value <= genetic_generator_get_cardinality(generator, i));
  }
  ga_individual_destroy(foo);
  genetic_generator_destroy(generator);
  return EXIT_SUCCESS;
}
