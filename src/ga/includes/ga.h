/**
    \file ga.h
    LIB GA - VOIR ga.h
*/
#ifndef SRC_GA_INCLUDES_GA_H_
#define SRC_GA_INCLUDES_GA_H_

#include <stdbool.h>
#include <stdio.h>


/**
 * \brief The Genetic Generator is used to create genes.
 *
 * Type Genetic Generator.
 * Used to create an individual's genes.
 * Contains an array of unsigned integers representing the maximum value of
 * every gene.
 */
typedef struct _GeneticGenerator GeneticGenerator;
typedef struct _Individial Individual;
typedef struct _Population Population;


/**
 * \brief Pointer to memory allocation function.
 *
 * By default, it points to stdlib's malloc.
 * By changing it you can force the library to use a different memory allocation
 * function than malloc.
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \var void *(*ga_malloc)(size_t size)
 * \param size size of the memory area to allocate.
 * \return a void pointer to the allocated memory location or NULL if failed.
 */
extern void *(*ga_malloc)(size_t size);
/**
 * \brief Pointer to memory reallocation function.
 *
 * By default, it points to stdlib's realloc.
 * By changing it you can force the library to use a different memory
 * reallocation function than realloc.
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \var void *(*ga_realloc)(void *ptr, size_t size)
 * \param ptr void pointer to memory location to reallocate.
 * \param size new size of the memory area to reallocate.
 * \return a void pointer to the reallocated memory location or NULL if failed.
 */
extern void *(*ga_realloc)(void *ptr, size_t size);
/**
 * \brief Pointer to memory freeing function.
 *
 * By default, it points to stdlib's free.
 * By changing it you can force the library to use a different memory
 * freeing function than free.
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \var void (*ga_free)(void *ptr)
 * \param ptr void pointer to memory location to free.
 */
extern void (*ga_free)(void *ptr);

/**
 *  \brief This function initializes the Genetic Algorithms library.
 *  \author Christophe Demko
 *  \version 0.0.1
 *  \date 2019
 *  \fn bool ga_init(void)
 *  \return true everytime.
 *
 *  This function must be used before any other function of the library.<br>
 *  In debug, it will write "GA initialised" to the standard output.<br>
 *  Always returns true.
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

#endif  // SRC_GA_INCLUDES_GA_H_
