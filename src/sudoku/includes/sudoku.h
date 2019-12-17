#ifndef SRC_SUDOKU_INCLUDES_SUDOKU_H_
#define SRC_SUDOKU_INCLUDES_SUDOKU_H_

typedef struct _Sudoku Sudoku;

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

#endif  // SRC_SUDOKU_INCLUDES_SUDOKU_H_
