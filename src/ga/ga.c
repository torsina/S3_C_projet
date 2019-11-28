#include "includes/ga.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ga.inc"

void *(*ga_malloc)(size_t size) = malloc;

void *(*ga_realloc)(void *ptr, size_t size) = realloc;

void (*ga_free)(void *ptr) = free;

static int counter = 0;

bool ga_init(void) {
  if (!counter++) {
    assert(printf("GA initialised\n"));
  }
  return true;
}

bool ga_finish(void) {
  if (counter) {
    if (!--counter) {
      assert(printf("GA finished\n"));
    }
    return true;
  } else {
    return false;
  }
}

GeneticGenerator *genetic_generator_create(const unsigned int size) {
  GeneticGenerator *generator = ga_malloc(sizeof *generator);
  if (generator) {
    generator->size = size;
    if (size) {
      generator->cardinalities = ga_malloc(sizeof(unsigned int) * size);
      if (!generator->cardinalities) {
        ga_free(generator);
        generator = NULL;
      } else {
        memset(generator->cardinalities, 0, sizeof(unsigned int) * size);
      }
    } else {
      generator->cardinalities = NULL;
    }
  }
  return generator;
}

void genetic_generator_destroy(GeneticGenerator *generator) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  ga_free(generator->cardinalities);
  ga_free(generator);
}

GeneticGenerator *genetic_generator_set_cardinality(
    GeneticGenerator *generator, const unsigned int index,
    const unsigned int cardinality) {
  if (generator && index < generator->size) {
    generator->cardinalities[index] = cardinality;
    return generator;
  } else {
    return NULL;
  }
}

unsigned int genetic_generator_get_cardinality(
    const GeneticGenerator *generator, const unsigned int index) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  // In debug : check if gene index is a valid index
  assert(index < generator->size);
  return generator->cardinalities[index];
}

unsigned int genetic_generator_get_size(const GeneticGenerator *generator) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  return generator->size;
}

GeneticGenerator *genetic_generator_clone(const GeneticGenerator *generator) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  GeneticGenerator *clone = genetic_generator_create(generator->size);
  if (clone) {
    memcpy(clone->cardinalities, generator->cardinalities,
           generator->size * sizeof(unsigned int));
    return clone;
  } else {
    return NULL;
  }
}

GeneticGenerator *genetic_generator_copy(GeneticGenerator *dest,
                                         const GeneticGenerator *src) {
  // In debug : check if the genetic generator destination pointer is not null
  assert(dest);
  // In debug : check if the genetic generator source pointer is not null
  assert(src);
  unsigned int *cardinalities =
      ga_realloc(dest->cardinalities, src->size * sizeof(unsigned int));
  if (cardinalities) {
    dest->size = src->size;
    dest->cardinalities = cardinalities;
    memcpy(dest->cardinalities, src->cardinalities,
           src->size * sizeof(unsigned int));
    return dest;
  } else {
    return NULL;
  }
}

GeneticGenerator *genetic_generator_fwrite(const GeneticGenerator *generator,
                                           FILE *stream) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  if (fwrite(&generator->size, sizeof(generator->size), 1, stream) == 1 &&
      fwrite(generator->cardinalities, sizeof(unsigned int), generator->size,
             stream) == generator->size) {
    return (GeneticGenerator *)generator;
  } else {
    return NULL;
  }
}

GeneticGenerator *genetic_generator_fread(GeneticGenerator *generator,
                                          FILE *stream) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  unsigned int size;
  unsigned int *cardinalities;
  if (fread(&size, sizeof(size), 1, stream) == 1) {
    cardinalities = ga_malloc(sizeof(unsigned int) * size);
    if (cardinalities) {
      if (fread(cardinalities, sizeof(unsigned int), size, stream) == size) {
        ga_free(generator->cardinalities);
        generator->size = size;
        generator->cardinalities = cardinalities;
        return generator;
      } else {
        ga_free(cardinalities);
        return NULL;
      }
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

static bool _add_string(char **pstring, unsigned int *plength,
                        const char *add) {
  if (add) {
    unsigned int inc_length = strlen(add);
    char *str = ga_realloc(*pstring, *plength + inc_length + 1);
    if (str) {
      *pstring = str;
      strncpy(*pstring + *plength, add, inc_length + 1);
      *plength += inc_length;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

const char *genetic_generator_to_string(const GeneticGenerator *generator) {
  // In debug : check if the genetic generator pointer is not null
  assert(generator);
  static char *str = NULL;
  unsigned int length;

  length = 0;

  if (!_add_string(&str, &length, "[")) {
    return NULL;
  }
  for (unsigned int index = 0; index < generator->size; index++) {
    if (index) {
      if (!_add_string(&str, &length, ",")) {
        return NULL;
      }
    }
    char element[100];
    snprintf(element, sizeof(element), "%u", generator->cardinalities[index]);
    if (!_add_string(&str, &length, element)) {
      return NULL;
    }
  }
  if (!_add_string(&str, &length, "]")) {
    return NULL;
  }
  return str;
}

Individual *genetic_generator_individual(const GeneticGenerator *generator) {
  // If the generator pointer is null, the individual can't be created
  if (!generator) {
    return NULL;
  }

  Individual *individual = ga_malloc(sizeof(Individual));
  if (individual) {
    individual->genes = ga_malloc(generator->size * sizeof(unsigned int));
    if (individual->genes) {
      for (unsigned int i = 0; i < generator->size; i++) {
        unsigned int value =
            generator->cardinalities[i] == 0
                ? 0
                : rand() % generator->cardinalities[i];  // NOLINT
        individual->genes[i] = value;
      }
      return individual;
    } else {
      ga_individual_destroy(individual);
      return NULL;
    }
  } else {
    return NULL;
  }
}

static unsigned int _ga_individual_get_gene(Individual *individual,
                                            unsigned int index) {
  // In debug : check if the individual pointer is not NULL
  assert(individual);
  return individual->genes[index];
}

void ga_individual_destroy(Individual *individual) {
  // In debug : check if the individual pointer is not NULL
  assert(individual);
  ga_free(individual->genes);
  ga_free(individual);
}

Population *ga_population_create(const GeneticGenerator *generator,
                                 unsigned int size) {
  if (generator) {
    Population *population = ga_malloc(sizeof(Population));
    if (population) {
      population->size = size;
      population->generator = (GeneticGenerator *)generator;
      population->individuals = ga_malloc(sizeof(Individual) * size);
      for (unsigned int i = 0; i < size; i++) {
        population->individuals[i] = genetic_generator_individual(generator);
      }
      return population;
    }
    return NULL;
  }
  return NULL;
}

void ga_population_destroy(Population *population) {
  // In debug : check if the population pointer is not null
  assert(population);
  for (unsigned int i = 0; i < population->size; i++) {
    // In debug : check if the individual pointer is not NULL
    assert(population->individuals[i]);
    ga_individual_destroy(population->individuals[i]);
  }
  ga_free(population->individuals);
  ga_free(population);
}

unsigned int ga_population_get_size(const Population *population) {
  // In debug : check if the population pointer is not null
  assert(population);
  return population->size;
}

GeneticGenerator *ga_population_get_generator(const Population *population) {
  // In debug : check if the population pointer is not null
  assert(population);
  // In debug : check if the genetic generator pointer is not null
  assert(population->generator);
  return (GeneticGenerator *)population->generator;
}

static Individual *_ga_population_get_individual(const Population *population,
                                                 unsigned int index) {
  if (population) {
    unsigned int nb_individuals = ga_population_get_size(population);
    if (index < nb_individuals) {
      return population->individuals[index];
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

static Population *_ga_population_set_individual(const Population *population,
                                                 unsigned int index,
                                                 const Individual *individual) {
  if (population) {
    unsigned int nb_individuals = ga_population_get_size(population);
    if (index < nb_individuals) {
      population->individuals[index] = (Individual *)individual;
      return (Population *)population;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

unsigned int ga_population_get_individual_gene(const Population *population,
                                               unsigned int individual_index,
                                               unsigned int gene_index) {
  // In debug : check if the population pointer is not null
  assert(population);
  GeneticGenerator *generator = ga_population_get_generator(population);
  // In debug : check if the genetic generator pointer is not null
  assert(generator);

  unsigned int nb_genes = genetic_generator_get_size(generator);

  Individual *individual =
      _ga_population_get_individual(population, individual_index);

  // In debug : check if the individual generator pointer is not null
  assert(individual);
  // In debug : check if gene index is a valid index
  assert(gene_index < nb_genes);
  return individual->genes[gene_index];
}

Population *ga_population_set_individual_gene(Population *population,
                                              unsigned int individual_index,
                                              unsigned int gene_index,
                                              unsigned int gene_value) {
  if (population) {
    GeneticGenerator *generator = ga_population_get_generator(population);
    if (generator) {
      unsigned int nb_genes = genetic_generator_get_size(generator);

      Individual *individual =
          _ga_population_get_individual(population, individual_index);
      if (individual && gene_index < nb_genes) {
        individual->genes[gene_index] = gene_value;
        return population;
      } else {
        return NULL;
      }
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

//[0..max]
/*static double random_double(double max) {
  return (double)rand() / (double)(RAND_MAX / max);
}*/