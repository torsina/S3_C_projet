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

static bool _check_mutate(const Population* pop, unsigned int index,
                          const Individual* old) {
  assert(pop);
  assert(old);
  const GeneticGenerator* gen = ga_population_get_generator(pop);
  assert(gen);
  printf("AAAAAAAAAA");
  unsigned int i = 0;
  unsigned int size = genetic_generator_get_size(gen);
  while (i < size &&
         old->genes[i] == ga_population_get_individual_gene(pop, index, i)) {
    i++;
  }

  if (i < size) {
    unsigned int index_of_mutated_gene = i;

    assert(
        ga_population_get_individual_gene(pop, index, index_of_mutated_gene) <
        genetic_generator_get_cardinality(gen, index_of_mutated_gene));

    i++;
    while (i < size &&
           old->genes[i] == ga_population_get_individual_gene(pop, index, i)) {
      i++;
    }
    if (i < size) {
      return false;
    } else {
      return true;
    }

  } else {
    return true;
  }
}

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
    assert(printf("Individual 0 :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", ga_population_get_individual_gene(population, 0, i));
    }
    printf("\n");

    const Individual* cloned = ga_individual_clone(population, 0);
    assert(cloned);

    mutate(population, 0);

    assert(printf("Individual 0  mutated :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", ga_population_get_individual_gene(population, 0, i));
    }
    printf("\n");

    assert(printf("Individual 0  cloned :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", cloned->genes[i]);
    }
    printf("\n");

    assert(_check_mutate(population, 0, cloned));

    ga_population_destroy(population);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
