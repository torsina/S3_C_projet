#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

#include "../../src/ga/includes/ga.h"

#include "../../src/ga/ga.inc"

// [0..max]
static double random_double(double max) {
  return (double)rand() / (double)(RAND_MAX / max);
}

static Individual **_array_list_add(Individual **array, unsigned int *size,
                                    Individual *val) {
  Individual **new_array = realloc(array, (*size + 1) * sizeof(Individual *));
  if (new_array) {
    new_array[*size] = val;
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
    return wheel->individuals[(unsigned int)_random_double(wheel->size - 1)];
  } else {
    return NULL;
  }
}

static int _pb[] = {20, 5, 1, 0, 1, 4};

unsigned int evaluate(Individual *individual, const void *pb) {
  unsigned int points = 0;
  for (unsigned int i = 0; i < 5; i++) {
    // TODO(T-MMLR) : fix data type
    points += individual->genes[i] - ((unsigned int *)pb)[i];
  }
  return points;
}

FortuneWheel *fortune_wheel(Population *population,
                            unsigned int (*evaluate)(Individual *,
                                                     const void *)) {
  if (!population) {
    return NULL;
  }
  if (!evaluate) {
    return NULL;
  }

  /* We create an array of the same size as the population to hold all of the
   scores of its individuals*/
  unsigned int scores[population->size];
  // This variable represents the sum of all the scores of a population
  unsigned int total = 0;
  // The algorithm iterates the individuals
  for (unsigned int i = 0; i < ga_population_get_size(population); i++) {
    // TODO(T-MMLR) : remplacer par _ga_individual_get_gene
    unsigned int score_int = evaluate(population->individuals[i], _pb);

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
   * initialised to NULL, but the _array_list_add will allocate memory for it
   * by calling realloc*/
  Individual **wheel = NULL;
  // Number of elements in the wheel
  unsigned int size = 0;
  // assert(printf("------------ROUE------------\n"));
  // assert(printf("[index] -> proba (nombre d'occurences dans la roue)\n"));
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

    /* By casting a double to an unsigned int we effectively obtain the whole
     part*/
    unsigned int whole_part = (unsigned int)n;
    double fractional_part = n - whole_part;

    // We add exactly "whole_part" times the Individual to the list
    for (unsigned int j = 0; j < whole_part; j++) {
      wheel = _array_list_add(wheel, &size, population->individuals[i]);
      // assert(printf("%u", whole_part));
    }
    /* Then, since we can't add 0.1 (or 0.5, 0.3, ...) times an Individual,
      we add it based on a probability : fractional_part

      This is the best solution since we can't just ignore the fractional part
      Ex : if an individual has a value of 0.9 -> it's almost 1, we can't just
      ignore it.
    */
    if (_random_double(1.0) < fractional_part) {
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

int main(void) {
  ga_init();
  {
    for (unsigned int i = 0; i < 5; i++) {
      printf("|%g", _random_double(1.0));
    }
    printf("\n");

    GeneticGenerator *generator = genetic_generator_create(5);
    genetic_generator_set_cardinality(generator, 0, 30);
    genetic_generator_set_cardinality(generator, 1, 10);
    genetic_generator_set_cardinality(generator, 2, 3);
    genetic_generator_set_cardinality(generator, 3, 0);
    genetic_generator_set_cardinality(generator, 4, 2);
    Population *population = ga_population_create(generator, 10);

    FortuneWheel *fortunewheel = fortune_wheel(population, evaluate);
    printf("Taille : %u\n", fortunewheel->size);
    for (unsigned int i = 0; i < fortunewheel->size; i++) {
      printf("%p ", fortunewheel->individuals[i]);
    }

    free(fortunewheel->individuals);
    free(fortunewheel);
    ga_population_destroy(population);
    genetic_generator_destroy(generator);
  }
  ga_finish();
  return EXIT_SUCCESS;
}
