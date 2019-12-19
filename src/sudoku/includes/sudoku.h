#include <stdio.h>
#include <stdbool.h>

#ifndef SRC_SUDOKU_INCLUDES_SUDOKU_H_
#define SRC_SUDOKU_INCLUDES_SUDOKU_H_

typedef struct _Sudoku Sudoku;

/**
 * \brief Returns the number of tiles in one square in the Sudoku grid.
 *
 * If a square has a width (and length) of n tiles, it contains nxn tiles.
 * There are nxn tiles in a Sudoku grid. The total number of tiles is :
 * N=(nxn)x(nxn)
 *
 * dim_size corresponds to nxn.
 * Example : in a "regular" sudoku, squares are made of 3x3 tiles.
 * dim_size = 9.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int sudoku_get_dim_size(const Sudoku* sudoku)
 * \param sudoku a pointer to the Sudoku to get the size from.
 * \return The number of tiles in one square (nxn).
 * \sa _evaluate_merge_problem_solution
 */
extern unsigned int sudoku_get_dim_size(const Sudoku* sudoku);

/**
 * \brief Creates the score this solution got on the Sudoku.
 *
 * This function create the score of a solution to the Sudoku
 * by counting the duplicates inside each row and column,
 * adding the counts together and subtracting that to the square of the number
 * of values in the Sudoku.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int evaluate(unsigned int* individual, const void* sudoku)
 * \param individual a pointer to the solution proposed for the problem.
 * \param sudoku a pointer to the Sudoku to solve.
 * \sa ga_population_next
 */
extern unsigned int evaluate(unsigned int* individual, const void* sudoku);

/**
 * \brief Allocates and initialize a new Sudoku struct.
 *
 * This function allocates memory space for a new Sudoku
 * and initialize it. If the allocation fails, a NULL pointer will be returned.
 * The parameter dim_size is used to define the size (number of tiles in a
 * square) of one square (dim_size = n²). In total there are dim_size² tiles.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn Sudoku *sudoku_create(unsigned int dim_size)
 * \param dim_size the tiles in 1 square (usually 9).
 * \return a Sudoku of the specified size with the grid initialized to 0,
 * or NULL if something goes wrong.
 * \sa sudoku_destroy
 */
extern Sudoku* sudoku_create(unsigned int dim_size);

/**
 * \brief Frees the memory used by a Sudoku struct.
 *
 * This function frees the memory used by a Sudoku struct
 * (pointed by the parameter sudoku) and its associated values.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn void sudoku_destroy(Sudoku *sudoku)
 * \param sudoku a pointer to the Sudoku to destroy.
 */
extern void sudoku_destroy(Sudoku* sudoku);


// TODO(T-MMLR) : remove
extern Sudoku* fill_sudoku(Sudoku* sudoku, FILE* file);

/**
 * \brief This function reads a Sudoku from a YAML file. The file must be
 * opened.
 *
 * This function returns NULL if the YAML syntax is invalid.
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn Sudoku* read_sudoku(FILE* file)
 * \param file an opened file handle.
 * \param verbose wether this function should print informations about its
 * execution to the standard output.
 * \return A Sudoku read from the parsed YAML file or NULL if something goes
 * wrong.
 */
extern Sudoku* read_sudoku(FILE* file, bool verbose);

/**
 * \brief This function returns the size of a potential solution (the number
 * of empty tiles, with the value 0).
 *
 *
 * \author Group 14
 * \version 0.0.1
 * \date 2019
 * \fn unsigned int sudoku_empty_tiles(const Sudoku *sudoku)
 * \param sudoku a pointer to the Sudoku to get the number of empty tiles from.
 * \return The number of empty tiles of the Sudoku grid.
 */
extern unsigned int sudoku_empty_tiles(const Sudoku* sudoku);


extern bool is_valid(unsigned int *solution, const Sudoku *sudoku);
extern bool is_max_score(unsigned int score, const Sudoku *sudoku);

extern void sudoku_print(unsigned int *solution, Sudoku *sudoku);
#endif  // SRC_SUDOKU_INCLUDES_SUDOKU_H_
