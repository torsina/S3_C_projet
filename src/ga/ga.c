#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ga.inc"
#include "includes/ga.h"

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
  ga_free(generator->cardinalities);
  ga_free(generator);
}

GeneticGenerator *genetic_generator_set_cardinality(
    GeneticGenerator *generator, const unsigned int index,
    const unsigned int cardinality) {
  assert(index < generator->size);
  generator->cardinalities[index] = cardinality;
  return generator;
}

unsigned int genetic_generator_get_cardinality(
    const GeneticGenerator *generator, const unsigned int index) {
  assert(index < generator->size);
  return generator->cardinalities[index];
}

unsigned int genetic_generator_get_size(const GeneticGenerator *generator) {
  return generator->size;
}

GeneticGenerator *genetic_generator_clone(const GeneticGenerator *generator) {
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
// our code
Individual *genetic_generator_individual(const GeneticGenerator *generator) {
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
    }
    return NULL;
  }
  return NULL;
}

unsigned int ga_individual_get_gene(Individual *individual,
                                    unsigned int index) {
  return individual->genes[index];
}

void ga_individual_destroy(Individual *individual) {
  ga_free(individual->genes);
  ga_free(individual);
}

Population *ga_population_create(const GeneticGenerator *generator,
                                 unsigned int size) {
  if (generator) {
    Population *population = ga_malloc(sizeof(Population));
    if (population) {
      population->size = size;
      population->generator = generator;
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

Population *ga_population_destroy(Population *population) {
  printf("started population destroy \n");
  for (unsigned int i = 0; i < population->size; i++) {
    printf("started individual destroy %u\n", i);
    ga_individual_destroy(population->individuals[i]);
    printf("finished individual destroy %u\n", i);
  }
  ga_free(population->individuals);
  ga_free(population);
  printf("finished population destroy \n");
  return NULL;
}
