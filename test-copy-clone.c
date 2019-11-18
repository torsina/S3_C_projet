/**
 * @file test-copy-clone.c
 *
 * @author     Christophe Demko <christophe.demko@univ-lr.fr>
 * @date       2019
 * @copyright  BSD 3-Clause License
 */

#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./ga.h"

int main(void) {
  ga_init();
  {
    GeneticGenerator* generator = genetic_generator_create(10);

    GeneticGenerator* clone = genetic_generator_clone(generator);
    assert(genetic_generator_get_size(generator) == genetic_generator_get_size(clone));
    genetic_generator_destroy(clone);

    GeneticGenerator* dest = genetic_generator_create(2);
    assert(genetic_generator_copy(dest, generator) == dest);
    assert(genetic_generator_get_size(generator) == genetic_generator_get_size(dest));
    genetic_generator_destroy(dest);

    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
