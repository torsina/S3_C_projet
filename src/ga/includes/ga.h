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
typedef struct _Individual Individual;
typedef struct _Population Population;

/**
 * \brief Pointer to memory allocation function.
 *
 * By default, it points to stdlib's malloc.
 * By changing it you can force the library to use a different memory allocation
 * function than malloc.
 *
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
 *
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
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \var void (*ga_free)(void *ptr)
 * \param ptr void pointer to memory location to free.
 */
extern void (*ga_free)(void *ptr);

/**
 *  \brief This function initializes the Genetic Algorithms library.
 *
 *  \author Christophe Demko
 *  \version 0.0.1
 *  \date 2019
 *  \fn bool ga_init(void)
 *  \return true everytime.
 *
 *  This function must be called before any other function of the library.<br>
 *  In debug, it writes "GA initialised" to the standard output.<br>
 *  Always returns true.
 */
extern bool ga_init(void);

/**
 *  \brief This function finalizes the Genetic Algorithms library.
 *
 *  This function must be called after any other function of the library.<br>
 *  In debug, it writes "GA finished" to the standard output.<br>
 *  Returns true until you called it the exact same number of time as
 *  ga_init, in this case it returns false only once.
 *
 *  \author Christophe Demko
 *  \version 0.0.1
 *  \date 2019
 *  \fn bool ga_finish(void)
 *  \return a boolean indicating if it has been called the same number of times
 *  as ga_init had been called.
 */
extern bool ga_finish(void);

/**
 * \brief Allocates and initialize a new GeneticGenerator struct.
 *
 * This function allocates memory space for a new GeneticGenerator
 * and initialize it. If the allocation fails, a NULL pointer will be returned.
 * The parameter size is be used to initialize the cardinalities to
 * 0 by default. You can then change each cardinality individually with
 * `genetic_generator_set_cardinality`.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *genetic_generator_create(const unsigned int size)
 * \param size the number of cardinalities (number of chromosomes).
 * \return a GeneticGenerator of the specified size with all the cardinalities
 * set to 0, or NULL if something goes wrong.
 * \sa genetic_generator_set_cardinality, genetic_generator_destroy
 */
extern GeneticGenerator *genetic_generator_create(const unsigned int size);

/**
 * \brief frees the memory used by a GeneticGenerator struct.
 *
 * This function frees the memory used by a GeneticGenerator struct
 * (pointed by the parameter generator) and its associated cardinalities.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn void genetic_generator_destroy(GeneticGenerator *generator)
 * \param generator a pointer to the GeneticGenerator to destroy.
 * \sa genetic_generator_create
 */
extern void genetic_generator_destroy(GeneticGenerator *generator);

/**
 * \brief setter for one chromosome cardinality of a genetic generator.
 *
 * This function sets the cardinality at the specified index of the
 * GeneticGenerator (passed through a pointer) to a new value.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *genetic_generator_set_cardinality(
 * GeneticGenerator *generator, const unsigned int index,
 * const unsigned int cardinality)
 * \param generator a pointer to the GeneticGenerator to modify.
 * \param index index of the chromosome cardinality to set.
 * \param cardinality value to set the chromosome cardinality to.
 * \return a pointer to the modified GeneticGenerator.
 * \sa genetic_generator_get_cardinality
 */
extern GeneticGenerator *genetic_generator_set_cardinality(
    GeneticGenerator *generator, const unsigned int index,
    const unsigned int cardinality);

/**
 * \brief getter for one chromosome cardinality of a genetic generator.
 *
 * This function returns the cardinality of a chromosome of a GeneticGenerator
 * at a specific index.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int genetic_generator_get_cardinality(
 * const GeneticGenerator *generator, const unsigned int index)
 * \param generator a pointer to the GeneticGenerator to get the cardinality
 * from.
 * \param index index of the chromosome cardinality to get.
 * \return the cardinality of the GeneticGenerator at the specified index.
 * \sa genetic_generator_set_cardinality
 */
extern unsigned int genetic_generator_get_cardinality(
    const GeneticGenerator *generator, const unsigned int index);

/**
 * \brief getter for the size of this GeneticGenerator
 *  (number of cardinalities).
 *
 * This function returns the number of chromosome cardinalities
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int genetic_generator_get_size(
 * const GeneticGenerator *generator)
 * \param generator a pointer to the GeneticGenerator to get the size from.
 * \return the size of the GeneticGenerator.
 * \sa genetic_generator_create
 */
extern unsigned int genetic_generator_get_size(
    const GeneticGenerator *generator);

/**
 * \brief clones the GeneticGenerator and returns a new GeneticGenerator
 * with the same values.
 *
 * This function returns a new GeneticGenerator, a deep copy of the
 * `genetic_generator` parameter.
 * The size and cardinalities should be identical between the original
 * and the cloned GeneticGenerator.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *genetic_generator_clone(
 * const GeneticGenerator *genetic_generator)
 * \param genetic_generator a pointer to the GeneticGenerator to clone.
 * \return a pointer to a new GeneticGenerator with the same values as
 * `genetic_generator` or a NULL pointer if something goes wrong.
 * \sa genetic_generator_copy
 */
extern GeneticGenerator *genetic_generator_clone(
    const GeneticGenerator *genetic_generator);

/**
 * \brief copies the values of `src` to `dest`.
 *
 * This function copies the values of `src` to `dest`. Returns a pointer to
 * the modified structure (`dest`) or a NULL pointer if something goes wrong.
 * The size and cardinalities of the returned GeneticGenerator should be
 * identical to the values of `src`.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *genetic_generator_copy(GeneticGenerator *dest,
 * const GeneticGenerator *src)
 * \param src a pointer to the GeneticGenerator to copy from.
 * \param dest a pointer to the GeneticGenerator to fill the values into.
 * \return a pointer to a GeneticGenerator with the same values as `src` or a
 * NULL pointer if something goes wrong.
 * \sa genetic_generator_clone
 */
extern GeneticGenerator *genetic_generator_copy(GeneticGenerator *dest,
                                                const GeneticGenerator *src);

/**
 * \brief writes the values of a GeneticGenerator to an opened byte stream.
 *
 * This function writes the size and cardinalities of a GeneticGenerator
 * to a byte stream. The stream must be opened with write permissions.
 * It returns `generator` if the operation is successful or a NULL pointer if
 * an error happens.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *genetic_generator_fwrite(
 * const GeneticGenerator *generator, FILE *stream)
 * \param generator a pointer to a GeneticGenerator to write to the stream.
 * \param stream a stdio `FILE` pointer representing the opened stream to write
 * the generator into.
 * \return a pointer to aGeneticGenerator with the same values as `generator`
 * or a NULL pointer if something goes wrong.
 * \sa genetic_generator_fread
 */
extern GeneticGenerator *genetic_generator_fwrite(
    const GeneticGenerator *generator, FILE *stream);

/**
 * \brief reads a GeneticGenerator from an opened byte stream.
 *
 * This function reads a GeneticGenerator from a byte stream.
 * The stream must be opened with write permissions.
 * It returns the modified `generator` with the values read from the file
 * (it automatically frees the previous values).
 * It can also return a NULL pointer if an error happens.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *genetic_generator_fread(GeneticGenerator *generator,
 * FILE *stream)
 * \param generator a pointer to a GeneticGenerator to put the read values into.
 * \param stream a stdio `FILE` pointer representing the opened stream to read
 * the generator from.
 * \return a pointer to a GeneticGenerator with the values read from the file
 * or a NULL pointer if something goes wrong.
 * \sa genetic_generator_fwrite
 */
extern GeneticGenerator *genetic_generator_fread(GeneticGenerator *generator,
                                                 FILE *stream);

/**
 * \brief returns a text representation of the GeneticGenerator.
 *
 * This function creates a text representation of the GeneticGenerator
 * with the following format :
 * \code{.c}
 *          [card 0,card 1,...,card size-1,card size]
 * \endcode
 * This function does not allocate a new char array every time. Instead it
 * relies on reusing the same "buffer" on every call.
 * The char array is return or, if an error happens, a NULL pointer is returned.
 *
 * \author Christophe Demko
 * \version 0.0.1
 * \date 2019
 * \fn const char *genetic_generator_to_string(
 * const GeneticGenerator *generator)
 * \param generator a pointer to a GeneticGenerator to create a text
 * representation from.
 * \return a const char array containing the textual representation of the
 * GeneticGenerator or a NULL pointer if something goes wrong.
 * \sa genetic_generator_fwrite
 */
extern const char *genetic_generator_to_string(
    const GeneticGenerator *generator);

/**
 * \brief Allocates and initialize a new individual struct.
 *
 * This function allocates memory space for a new individual
 * and initialize it. If the allocation fails, a NULL pointer will be returned.
 * For each slot in the genes table, it will check at the same index
 * in the cardinalities table of generator if the number in the slot is 0,
 * if it's the case, a 0 will be written in the genes slot,
 * if not, a random number between 0 and the number in the
 * cardinalities slot will be written in the genes slot.
 * The parameter generator is be used to initialize the length of the genes
 * table.(this length is obtained with the size attribute of the generator).
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn Individual *genetic_generator_individual(
 * const GeneticGenerator *generator)
 * \param generator a pointer to the GeneticGenerator to deal with.
 * \return an Individual with the specified size with the genes table fully
 * filled, or NULL if something goes wrong..
 * \sa ga_individual_destroy
 */

extern Individual *genetic_generator_individual(
    const GeneticGenerator *generator);

/**
 * \brief frees the memory used by a Individual struct.
 *
 * This function frees the memory used by a Individual struct
 * (pointed by the parameter individual) and its associated genes.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn void ga_individual_destroy(Individual *individual)
 * \param individual a pointer to the Individual to destroy.
 * \sa genetic_generator_individual
 */
extern void ga_individual_destroy(Individual *individual);

// Population functions
/**
 * \brief Allocates and initialize a new Population struct.
 *
 * This function allocates memory space for a new Population
 * and initialize it.
 * If the allocation fails, a NULL pointer will be returned.
 * The parameter size is be used to initialize the size of the Individual table.
 * The parameter generator is be used to add a own GeneticGenerator to the
 * Population struct.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn Population *ga_population_create(
 * const GeneticGenerator *generator,unsigned int size)
 * \param generator the pointer to a GeneticGenerator used to create
 * Individuals.
 * \param size the the size of the Individual table.
 * \return a Population with the specified size, generator and  with all the
 * Individuals created, or NULL if something goes wrong.
 * \sa genetic_generator_individual, ga_population_destroy
 */
extern Population *ga_population_create(const GeneticGenerator *generator,
                                        unsigned int size);
/**
 * \brief frees the memory used by a Population struct.
 *
 * This function frees the memory used by a Population struct
 * (pointed by the parameter population) and its associated elements
 * (table of pointers to Individuals).
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn Population *ga_population_destroy(Population *population)
 * \param population a pointer to the Population to destroy.
 * \sa ga_population_create, genetic_generator_individual
 */
extern void ga_population_destroy(Population *population);

// New functions (getters and setters) for population
/**
 * \brief getter for the size of a Population.
 *
 * This function returns the size attribute of a Population.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int ga_population_get_size(const Population *population)
 * \param population a pointer to the Population to get the size from.
 * \return the size of the Population.
 * \sa ga_population_get_generator, ga_population_get_individual_gene,
 * ga_population_set_individual_gene
 */
extern unsigned int ga_population_get_size(const Population *population);

/**
 * \brief getter for the generator of a Population.
 *
 * This function returns the generator pointer of a Population.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn GeneticGenerator *ga_population_get_generator(
 * const Population *population)
 * \param population a pointer to the Population to get the generator from.
 * \return the generator pointer of the Population.
 * \sa ga_population_get_size, ga_population_get_individual_gene,
 * ga_population_set_individual_gene
 */
extern GeneticGenerator *ga_population_get_generator(
    const Population *population);

/**
 * \brief getter for the value in the genes table (at the specified index)
 * of the Individual owned by the Population.
 *
 * This function returns the int value wanted in the genes table
 * of the Individual owned by the Population.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int ga_population_get_individual_gene(
 * const Population *population, unsigned int individual_index,
 * unsigned int gene_index)
 * \param population a pointer to the Population to get the generator from.
 * \param individual_index an index to get a specific Individual from the
 * individuals attribute of the Population.
 * \param gene_index the index of the slot where we want
 * to get the gene_value.
 * \return the value wanted in the genes table of the Individual owned by
 * the Population.
 * \sa ga_population_set_individual_gene
 */
extern unsigned int ga_population_get_individual_gene(
    const Population *population, unsigned int individual_index,
    unsigned int gene_index);

/**
 * \brief setter for for the value in the genes table (at the specified index)
 * of the Individual owned by the Population.
 *
 * This function sets the int value wanted in the genes table
 * of the Individual owned by the Population.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn Population *ga_population_set_individual_gene(
 * Population *population, unsigned int individual_index,
 * unsigned int gene_index, unsigned int gene_value)
 * \param population a pointer to the Population to get the generator from.
 * \param individual_index an index to get a specific Individual from the
 * individuals attribute of the Population.
 * \param gene_index the index of the slot where we want
 * to insert the gene_value.
 * \param gene_value the value that we want to insert in the genes table.
 * \return the gene_value set in the genes table of the Individual owned by
 * the Population.
 * \sa ga_population_get_individual_gene
 */
extern Population *ga_population_set_individual_gene(
    Population *population, unsigned int individual_index,
    unsigned int gene_index, unsigned int gene_value);

#endif  // SRC_GA_INCLUDES_GA_H_
