#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/ga/includes/ga.h"

int main(void) {
  GeneticGenerator* generator = genetic_generator_create(2);
  genetic_generator_set_cardinality(generator, 0, 50);
  genetic_generator_set_cardinality(generator, 1, 12);
  Individual* foo = genetic_generator_individual(generator);
  unsigned int first = ga_individual_get_gene(foo, 0);
  unsigned int second = ga_individual_get_gene(foo, 1);
  assert(first >= 0 &&
         first <= genetic_generator_get_cardinality(generator, 0));
  assert(second >= 0 &&
         second <= genetic_generator_get_cardinality(generator, 1));
  ga_individual_destroy(foo);
  return EXIT_SUCCESS;
}
