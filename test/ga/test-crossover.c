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
                             unsigned int p2, const Individual* child) {
  assert(pop);
  assert(child);
  const GeneticGenerator* gen = ga_population_get_generator(pop);
  assert(gen);

  unsigned int i = 0;
  unsigned int size = genetic_generator_get_size(gen);
  /* Iterate over the child's genes and check if every gene is equal to p1's
   * gene.
   */
  while (i < size &&
         child->genes[i] == ga_population_get_individual_gene(pop, p1, i)) {
    i++;
  }
  /*If the loop exits, we check why :
   * - if (i < size) it means that we stumbled upon a gene in "child" which
   *    is different from p1's gene. We can continue by checking p2's genes.
   * - otherwise, we reached the end of the "genes" array and every gene of
   *    "child" equals p1's genes.
   */
  if (i < size) {
    /* Iterate over the child's genes and check if every gene is equal to p2's
     * gene.
     */
    while (i < size &&
           child->genes[i] == ga_population_get_individual_gene(pop, p2, i)) {
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

    // Test crossover
    assert(printf("Individual 0 :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", ga_population_get_individual_gene(population, 0, i));
    }
    assert(printf("\nIndividual 1 :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", ga_population_get_individual_gene(population, 1, i));
    }
    assert(printf("\n"));

    Individual* child = crossover(population, 0, 1);
    assert(child);

    assert(printf("\nChild of 0 & 1 :\n"));
    for (unsigned int i = 0; i < genetic_generator_get_size(generator); i++) {
      printf("%u|", child->genes[i]);
    }
    assert(printf("\n"));
    assert(_check_crossover(population, 0, 1, child));

    ga_individual_destroy(child);
    ga_population_destroy(population);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
