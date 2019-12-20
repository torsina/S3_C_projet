#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdbool.h>

#include "../../src/ga/includes/ga.h"

// Useful for working with individuals outside populations. Only for tests.
#include "../../src/ga/ga.inc"

static bool _check_crossover(const Population* pop, unsigned int p1,
                             unsigned int p2, const Individual* clone0,
                             const Individual* clone1) {
  assert(pop);
  assert(clone0);
  assert(clone1);
  const GeneticGenerator* gen = ga_population_get_generator(pop);
  assert(gen);

  unsigned int i = 0;
  unsigned int size = genetic_generator_get_size(gen);
  /* Iterate over the clone0's genes and check if every gene is equal to p1's
   * gene.
   */

  bool wrong_gene_found = false;
  while (i < size && !wrong_gene_found) {
    /* If we find a gene in the new population that comes from neither clone0
     nor clone1*/
    if ((ga_population_get_individual_gene(pop, p1, i) != clone0->genes[i] &&
         ga_population_get_individual_gene(pop, p1, i) != clone1->genes[i]) ||
        (ga_population_get_individual_gene(pop, p2, i) != clone0->genes[i] &&
         ga_population_get_individual_gene(pop, p2, i) != clone1->genes[i])) {
      wrong_gene_found = true;
    }
    i++;
  }
  return !wrong_gene_found;
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

    // Test crossover
    assert(printf("Individual 0 :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", ga_population_get_individual_gene(population, 0, i));
    }

    Individual* clone0 = ga_population_individual_clone(population, 0);
    assert(clone0);
    Individual* clone1 = ga_population_individual_clone(population, 1);
    assert(clone1);

    assert(printf("\nIndividual 1 :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", ga_population_get_individual_gene(population, 1, i));
    }
    assert(printf("\n"));

    Population* next_gen = crossover(population, 0, 1, 0.1f);
    assert(next_gen);

    assert(_check_crossover(population, 0, 1, clone0, clone1));

    ga_individual_destroy(clone0);
    ga_individual_destroy(clone1);
    ga_population_destroy(population);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
