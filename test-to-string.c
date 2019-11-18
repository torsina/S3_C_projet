/**
 * @file test-to-string.c
 *
 * @author     Christophe Demko <christophe.demko@univ-lr.fr>
 * @date       2019
 * @copyright  BSD 3-Clause License
 */

#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./ga.h"

int main(void) {
  ga_init();
  {
    GeneticGenerator* generator = genetic_generator_create(10);
    genetic_generator_set_cardinality(generator, 0, 5);
    assert(strcmp(genetic_generator_to_string(generator), "[5,0,0,0,0,0,0,0,0,0]") == 0);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
