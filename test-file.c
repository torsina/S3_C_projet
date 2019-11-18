/**
 * @file test-file.c
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
    genetic_generator_set_cardinality(generator, 0, 5);
    GeneticGenerator* read = genetic_generator_create(0);

    FILE* stream = tmpfile();
    assert(genetic_generator_fwrite(generator, stream) == generator);
    rewind(stream);

    assert(genetic_generator_fread(read, stream) == read);
    assert(genetic_generator_get_size(generator) == genetic_generator_get_size(read));
    for (unsigned int index = 0; index < genetic_generator_get_size(generator); index++) {
      assert(genetic_generator_get_cardinality(generator, index) == genetic_generator_get_cardinality(read, index));
    }
    fclose(stream);

    stream = tmpfile();
    assert(genetic_generator_fread(read, stream) == NULL);
    fclose(stream);

    genetic_generator_destroy(generator);
    genetic_generator_destroy(read);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
