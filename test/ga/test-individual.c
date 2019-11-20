#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/ga/includes/ga.h"

int main(void) {
  GeneticGenerator* generator = genetic_generator_create(3);
  genetic_generator_set_cardinality(generator, 0, 2);
  genetic_generator_set_cardinality(generator, 1, 2);
  genetic_generator_set_cardinality(generator, 2, 2);
  Individual* foo = genetic_generator_individual(generator);
  assert()
  ga_individual_destroy(foo);
  return EXIT_SUCCESS;
}
