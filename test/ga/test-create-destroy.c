/**
 * @file test-create-destroy.c
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

#include "../../src/ga/includes/ga.h"

int main(void) {
  ga_init();
  {
    GeneticGenerator* generator = genetic_generator_create(10);
    assert(genetic_generator_get_size(generator) == 10);
    for (unsigned int index = 0; index < genetic_generator_get_size(generator);
         index++) {
      assert(genetic_generator_get_cardinality(generator, index) == 0);
    }
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
