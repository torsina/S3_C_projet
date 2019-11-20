#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

#include "../../src/ga/includes/ga.h"

#include "../../src/ga/ga.inc"

int main(void) {
  ga_init();
  {
    const unsigned int NB_CHROMOSOMES = 20;
    GeneticGenerator* generator = genetic_generator_create(NB_CHROMOSOMES);
    assert(generator);
    assert(genetic_generator_get_size(generator) == NB_CHROMOSOMES);
    for (unsigned int i = 0; i < NB_CHROMOSOMES; i++) {
      assert(genetic_generator_set_cardinality(generator, i,
                                               i * NB_CHROMOSOMES + 1));
    }
    Individual* foo = genetic_generator_individual(generator);
    assert(foo);
    for (unsigned int i = 0; i < NB_CHROMOSOMES; i++) {
      unsigned int value = foo->genes[i];
      assert(value >= 0 &&
             value <= genetic_generator_get_cardinality(generator, i));
    }
    ga_individual_destroy(foo);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
