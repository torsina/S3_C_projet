#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "../../src/ga/ga.inc"
#include "../../src/ga/includes/ga.h"

int main(void) {
  ga_init();
  GeneticGenerator* generator = genetic_generator_create(20);
  for (unsigned int i = 0; i < 20; i++) {
    genetic_generator_set_cardinality(generator, i, i * 20 + 1);
  }
  Individual* foo = genetic_generator_individual(generator);
  for (unsigned int i = 0; i < 20; i++) {
    unsigned int value = foo->genes[i];
    assert(value >= 0 &&
           value <= genetic_generator_get_cardinality(generator, i));
  }
  ga_individual_destroy(foo);
  genetic_generator_destroy(generator);
  ga_finish();
  // TODO: abc
  return EXIT_SUCCESS;
}
