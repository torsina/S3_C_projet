#include "includes/ga.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ga.inc"

void *(*ga_malloc)(size_t size) = malloc;

void *(*ga_realloc)(void *ptr, size_t size) = realloc;

void (*ga_free)(void *ptr) = free;

static int counter = 0;

bool ga_init(void) {
  if (!counter++) {
    assert(printf("GA initialised\n"));
    srand(time(NULL));
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

Individual *ga_population_individual_clone(const Population *pop,
                                           unsigned int index) {
  assert(pop);
  assert(index < ga_population_get_size(pop));

  return ga_individual_clone(pop->individuals[index],
                             ga_population_get_generator(pop));
  /*TODO(T-MMLR) : test and remove
  Individual *clone = ga_malloc(sizeof *clone);
  if (clone) {
    size_t nb = genetic_generator_get_size(ga_population_get_generator(pop));
    unsigned int *genes = ga_malloc(sizeof(unsigned int) * nb);
    if (genes) {
      memcpy(genes, pop->individuals[index]->genes, nb * sizeof(unsigned int));
      clone->genes = genes;
      return clone;
    } else {
      free(clone);
      return NULL;
    }
  } else {
    return NULL;
  }*/
}

Individual *ga_individual_clone(const Individual *src,
                                const GeneticGenerator *generator) {
  if (!src || !generator) {
    return NULL;
  }

  Individual *clone = ga_malloc(sizeof *clone);
  if (clone) {
    size_t nb = genetic_generator_get_size(generator);
    unsigned int *genes = ga_malloc(sizeof(unsigned int) * nb);
    if (genes) {
      memcpy(genes, src->genes, nb * sizeof(unsigned int));
      clone->genes = genes;
      return clone;
    } else {
      free(clone);
      return NULL;
    }
  } else {
    return NULL;
  }
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
// Return the individual of a population with the following index
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
/*Set the individual of a population with the following index and with the
 individual to set*/
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

// Return a pseudo-random double between 0 and max included
static double random_double(double max) {
  return (double)rand() / (double)(RAND_MAX / max);
}

Individual *mutate(Population *population, unsigned int individual_index) {
  if (population) {
    GeneticGenerator *generator = ga_population_get_generator(population);
    if (generator) {
      unsigned int nb_genes = genetic_generator_get_size(generator);
      unsigned int gene_index = (int)random_double(nb_genes - 1);
      unsigned int gene_value = (int)random_double(
          genetic_generator_get_cardinality(generator, gene_index) - 1);
      Individual *individual =
          _ga_population_get_individual(population, individual_index);
      if (individual && gene_index < nb_genes) {
        if (ga_population_set_individual_gene(population, individual_index,
                                              gene_index, gene_value)) {
          return individual;
        } else {
          return NULL;
        }
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

Individual *crossover(const Population *population,
                      unsigned int first_individual_index,
                      unsigned int second_individual_index) {
  if (population) {
    GeneticGenerator *generator = ga_population_get_generator(population);
    if (generator) {
      unsigned int nb_genes = genetic_generator_get_size(generator);
      unsigned int gene_pivot_index = (int)random_double(nb_genes - 1);
      Individual *first_individual =
          _ga_population_get_individual(population, first_individual_index);
      Individual *second_individual =
          _ga_population_get_individual(population, second_individual_index);
      if (first_individual && second_individual &&
          gene_pivot_index < nb_genes) {
        Individual *child_individual = ga_malloc(sizeof(Individual));
        if (child_individual) {
          child_individual->genes =
              ga_malloc(generator->size * sizeof(unsigned int));
          if (child_individual->genes) {
            for (unsigned int i = 0; i < gene_pivot_index; i++) {
              unsigned int value = first_individual->genes[i];
              child_individual->genes[i] = value;
            }
            for (unsigned int i = gene_pivot_index; i < generator->size; i++) {
              unsigned int value = second_individual->genes[i];
              child_individual->genes[i] = value;
            }
            return child_individual;
          } else {
            ga_individual_destroy(child_individual);
            return NULL;
          }
        } else {
          return NULL;
        }
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

Population *crossover_2(const Population *population,
                        unsigned int first_individual_index,
                        unsigned int second_individual_index) {
  if (population) {
    GeneticGenerator *generator = ga_population_get_generator(population);
    if (generator) {
      unsigned int nb_genes = genetic_generator_get_size(generator);
      unsigned int gene_pivot_index = (int)random_double(nb_genes - 1);
      Individual *first_individual =
          _ga_population_get_individual(population, first_individual_index);
      Individual *second_individual =
          _ga_population_get_individual(population, second_individual_index);
      if (first_individual && second_individual &&
          gene_pivot_index < nb_genes) {
        unsigned int gene1 =
            _ga_individual_get_gene(first_individual, gene_pivot_index);
        // Swapping genes
        ga_population_set_individual_gene(
            (Population *)population, first_individual_index, gene_pivot_index,
            _ga_individual_get_gene(second_individual, gene_pivot_index));
        ga_population_set_individual_gene((Population *)population,
                                          second_individual_index,
                                          gene_pivot_index, gene1);
        return (Population *)population;
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

/**
 * \brief This function adds an Individual to an array of individuals and
 * increases its size.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn static Individual **_array_list_add(Individual **array, unsigned int
 *size, const Individual *val)
 * \param size a pointer to the current size of the array. Will be modified
 * and increased once the Individual is added.
 * \param val a pointer to an Individual to add to the array.
 * \return The array, modified or NULL if something goes wrong.
 * \sa fortune_wheel
 */
static Individual **_array_list_add(Individual **array, unsigned int *size,
                                    const Individual *val) {
  Individual **new_array = realloc(array, (*size + 1) * sizeof(Individual *));
  if (new_array) {
    new_array[*size] = (Individual *)val;
    (*size)++;
    return new_array;
  } else {
    *size = 0;
    return NULL;
  }
}

/**
 * \brief This function randomly selects an Individual from an already
 * generated FortuneWheel.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn static Individual *_fortune_wheel_draw(FortuneWheel *wheel)
 * \param wheel a pointer to a FortuneWheel.
 * \return A pointer to a randomly chosen Individual among the wheel.
 * \sa fortune_wheel
 */
static Individual *_fortune_wheel_draw(FortuneWheel *wheel) {
  if (wheel) {
    return wheel->individuals[(unsigned int)random_double(wheel->size - 1)];
  } else {
    return NULL;
  }
}

/**
 * \brief This function creates a FortuneWheel from a given population, a
 * a function to compute the score of an Individual and the problem to solve.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn static FortuneWheel *_fortune_wheel(Population *population,
                            unsigned int (*evaluate)(Individual *,
                                                     const void *),
                            const void *problem)
 * \param population a pointer to a Population containing the individuals.
 * \param evaluate a pointer to a function used to evaluate each Individual
 * and getting its score relative to the problem.
 * \param problem a pointer to the problem to solve, passed to evaluate.
 * \return A pointer to a FortuneWheel or NULL if something went wrong.
 * \sa _fortune_wheel_draw
 */
static FortuneWheel *_fortune_wheel(Population *population,
                                    unsigned int (*evaluate)(unsigned int *,
                                                             const void *),
                                    const void *problem) {
  if (!population) {
    return NULL;
  }
  if (!evaluate) {
    return NULL;
  }

  /* We create an array of the same size as the population to hold all of
   the scores of its individuals*/
  unsigned int scores[population->size];
  // This variable represents the sum of all the scores of a population
  unsigned int total = 0;
  // The algorithm iterates the individuals
  for (unsigned int i = 0; i < ga_population_get_size(population); i++) {
    unsigned int score_int =
        evaluate(_ga_population_get_individual(population, i)->genes, problem);

    // We add the score of the current Individual to the total
    total += score_int;
    // We store the score of the current Individual in the array
    scores[i] = score_int;
    // assert(printf("%p : %u", population->individuals[i], score_int));
    /*if (i != ga_population_get_size(population) - 1) {
      assert(printf(", "));
    }*/
  }

  // assert(printf("\n"));

  /* This is the fortune wheel (an array of individuals). By default, it is
   * initialised to NULL, but the _array_list_add will allocate memory for
   * it by calling realloc*/
  Individual **wheel = NULL;
  // Number of elements in the wheel
  unsigned int size = 0;
  // assert(printf("------------ROUE------------\n"));
  // assert(printf("[index] -> proba (nombre d'occurences dans la
  // roue)\n"));
  for (unsigned int i = 0; i < ga_population_get_size(population); i++) {
    /* The probability for an individual to be chosen is equal to :
     * P=(score / total_score)
     *
     * In order to create a fortune wheel we repeat a certain number of
     * times n an Individual based on its probability :
     * n=P*population_size
     */
    double n = (double)scores[i] / total * ga_population_get_size(population);
    // assert(printf("[%u] -> %g (", i, prob));

    /* By casting a double to an unsigned int we effectively obtain the
     whole part*/
    unsigned int whole_part = (unsigned int)n;
    double fractional_part = n - whole_part;

    // We add exactly "whole_part" times the Individual to the list
    for (unsigned int j = 0; j < whole_part; j++) {
      wheel = _array_list_add(wheel, &size, population->individuals[i]);
      // assert(printf("%u", whole_part));
    }
    /* Then, since we can't add 0.1 (or 0.5, 0.3, ...) times an Individual,
      we add it based on a probability : fractional_part

      This is the best solution since we can't just ignore the fractional
      part Ex : if an individual has a value of 0.9 -> it's almost 1, we
      can't just ignore it.
    */
    if (random_double(1.0) < fractional_part) {
      wheel = _array_list_add(wheel, &size, population->individuals[i]);
      // assert(printf("+1"));
    }
    // assert(printf(")\n"));

    /*if (i != ga_population_get_size(population) - 1) {
      printf(", ");
    }*/
  }
  // assert(printf("----------------------------\n"));
  if (wheel) {
    FortuneWheel *fortune_wheel = malloc(sizeof *fortune_wheel);
    fortune_wheel->size = size;
    fortune_wheel->individuals = wheel;
    return fortune_wheel;
  } else {
    return NULL;
  }
}

Population *ga_population_next(Population *population, const float cross_over,
                               const float mutation,
                               unsigned int (*evaluate)(unsigned int *,
                                                        const void *),
                               const void *problem) {
  if (population && evaluate && problem) {
    if (cross_over < 0.0f || cross_over > 1.0 || mutation < 0.0f ||
        mutation > 1.0f) {
      return NULL;
    } else {
      // Generating the fortune wheel for this population.
      FortuneWheel *wheel = _fortune_wheel(population, evaluate, problem);

      if (wheel && wheel->size > 0) {
        Population *next_generation =
            ga_population_create(ga_population_get_generator(population),
                                 ga_population_get_size(population));
        // The size is meant to be even
        if (next_generation &&
            ga_population_get_size(next_generation) % 2 == 0) {
          unsigned int current_size = 0;

          // The size is meant to be even
          while (current_size < ga_population_get_size(next_generation)) {
            Individual *individual1 =
                ga_individual_clone(_fortune_wheel_draw(wheel),
                                    ga_population_get_generator(population));
            Individual *individual2 =
                ga_individual_clone(_fortune_wheel_draw(wheel),
                                    ga_population_get_generator(population));
            if (individual1 && individual2) {
              /* The postfix increment operator increments after passing the
               value to the function*/
              _ga_population_set_individual(next_generation, current_size++,
                                            individual1);
              _ga_population_set_individual(next_generation, current_size++,
                                            individual2);
              // If we want to use the default "crossover", remove these lines

              if (random_double(1.0) < cross_over) {
                crossover_2(next_generation, current_size - 2,
                            current_size - 1);
              }

              // Random mutation on the 2 individual
              if (random_double(1.0) < mutation) {
                mutate(next_generation, current_size - 2);
              }

              if (random_double(1.0) < mutation) {
                mutate(next_generation, current_size - 1);
              }
            } else {
              if (individual1)
                ga_individual_destroy(individual1);
              if (individual2)
                ga_individual_destroy(individual2);
            }
          }

          free(wheel->individuals);
          free(wheel);

          ga_population_destroy(population);

          return next_generation;
        } else {
          free(wheel->individuals);
          free(wheel);
          ga_population_destroy(next_generation);
          return NULL;
        }
      } else {
        return NULL;
      }
    }
  } else {
    return NULL;
  }
}
