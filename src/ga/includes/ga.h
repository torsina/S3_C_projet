/**
    \file ga.h
    LIB GA - VOIR ga.h
*/
#ifndef SRC_GA_INCLUDES_GA_H_
#define SRC_GA_INCLUDES_GA_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct _GeneticGenerator GeneticGenerator;
typedef struct _Individual Individual;
typedef struct _Population Population;

extern void *(*ga_malloc)(size_t size);
extern void *(*ga_realloc)(void *ptr, size_t size);
extern void (*ga_free)(void *ptr);

/**
 *  \brief Cette fonction calcule
 *         la racine carrée.
 *  \author Francis
 *  \version 1.2.0
 *  \date 12 octobre 2019
 *  \fn bool ga_init(void)
 *  \return La racine carrée de val.
 */
extern bool ga_init(void);
extern bool ga_finish(void);

extern GeneticGenerator *genetic_generator_create(const unsigned int size);
extern void genetic_generator_destroy(GeneticGenerator *generator);

extern GeneticGenerator *genetic_generator_set_cardinality(
    GeneticGenerator *generator, const unsigned int index,
    const unsigned int cardinality);
extern unsigned int genetic_generator_get_cardinality(
    const GeneticGenerator *generator, const unsigned int index);
extern unsigned int genetic_generator_get_size(
    const GeneticGenerator *generator);

extern GeneticGenerator *genetic_generator_clone(
    const GeneticGenerator *genetic_generator);
extern GeneticGenerator *genetic_generator_copy(GeneticGenerator *dest,
                                                const GeneticGenerator *src);

extern GeneticGenerator *genetic_generator_fwrite(
    const GeneticGenerator *generator, FILE *stream);
extern GeneticGenerator *genetic_generator_fread(GeneticGenerator *generator,
                                                 FILE *stream);

extern const char *genetic_generator_to_string(
    const GeneticGenerator *generator);

extern Individual *genetic_generator_individual(
    const GeneticGenerator *generator);

extern void ga_individual_destroy(Individual *individual);

extern unsigned int ga_individual_get_gene(Individual *individual,
                                           unsigned int index);

extern Population *ga_population_create(const GeneticGenerator *generator,
                                        unsigned int size);

#endif  // SRC_GA_INCLUDES_GA_H_
