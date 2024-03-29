#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../src/ga/includes/ga.h"

// Useful for working with individuals outside populations. Only for tests.
#include "../../src/ga/ga.inc"

int main(void) {
  ga_init();
  {
    const unsigned int NB_CHROMOSOMES = 10;
    const unsigned int NB_INDIVIDUALS = 10;
    GeneticGenerator* generator = genetic_generator_create(NB_CHROMOSOMES);
    assert(generator);
    assert(genetic_generator_get_size(generator) == NB_CHROMOSOMES);
    for (unsigned int cardinality_index = 0; cardinality_index < NB_CHROMOSOMES;
         cardinality_index++) {
      assert(genetic_generator_set_cardinality(generator, cardinality_index,
                                               cardinality_index + 1));
    }

    Population* population = ga_population_create(generator, NB_INDIVIDUALS);
    assert(population);

    // Test mutate
    // assert(printf("Individual 0 :\n"));
    // for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++)
    // { assert(printf("%u|", ga_population_get_individual_gene(population, 0,
    // i)));
    // }
    // assert(printf("\n"));

    const Individual* cloned = ga_population_individual_clone(population, 0);
    assert(cloned);

    mutate(population, 0, 0.1f);

    // assert(printf("Individual 0  mutated :\n"));
    // for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++)
    // { assert(printf("%u|", ga_population_get_individual_gene(population, 0,
    // i)));
    // }
    // assert(printf("\n"));

    // assert(printf("Individual 0  cloned :\n"));
    // for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++)
    // { assert(printf("%u|", cloned->genes[i]));
    // }
    // assert(printf("\n"));

    assert(population);
    assert(population->individuals[0]);

    // We must free the memory allocated to the cloned Individual
    ga_individual_destroy((Individual*)cloned);
    ga_population_destroy(population);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
