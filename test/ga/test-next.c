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

static void _display_population(Population* population) {
  assert(population);
  const GeneticGenerator* gen = ga_population_get_generator(population);
  assert(gen);

  for (unsigned int i = 0; i < ga_population_get_size(population); i++) {
    printf("\t");
    for (unsigned int j = 0; j < genetic_generator_get_size(gen); j++) {
      printf("%u|", ga_population_get_individual_gene(population, i, j));
    }
    printf("\n");
  }
}

typedef struct {
  unsigned int* values;
  unsigned int size;
} Problem;

static unsigned int evaluate(unsigned int* individual, const void* problem) {
  unsigned int score = 0;
  Problem* pb = (Problem*)problem;
  for (unsigned int i = 0; i < pb->size; i++) {
    if (individual[i] == pb->values[i]) {
      score++;
    }
  }
  return score;
}

int main(void) {
  ga_init();
  {
    const unsigned int NB_CHROMOSOMES = 10;
    const unsigned int NB_INDIVIDUALS = 10;

    // Creating the generator
    GeneticGenerator* generator = genetic_generator_create(NB_CHROMOSOMES);
    assert(generator);
    assert(genetic_generator_get_size(generator) == NB_CHROMOSOMES);

    // Setting all the cardinalities of the generator
    for (unsigned int cardinality_index = 0; cardinality_index < NB_CHROMOSOMES;
         cardinality_index++) {
      assert(genetic_generator_set_cardinality(generator, cardinality_index,
                                               cardinality_index + 1));
    }

    // Creating a Population of individuals
    Population* population = ga_population_create(generator, NB_INDIVIDUALS);
    assert(population);

    printf("Generation 0 :\n");
    _display_population(population);
    printf("-------------------------------------\n");

    Problem pb;
    pb.size = NB_CHROMOSOMES;
    pb.values = calloc(sizeof(unsigned int), NB_CHROMOSOMES);

    Population* new_pop =
        ga_population_next(population, 0.0f, 0.0f, &evaluate, &pb);

    assert(new_pop);
    population = new_pop;

    printf("Generation 1 :\n");
    _display_population(population);
    printf("-------------------------------------\n");

    ga_free(pb.values);
    ga_population_destroy(population);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
