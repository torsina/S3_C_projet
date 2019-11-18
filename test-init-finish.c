/**
 * @file test-init-finish.c
 *
 * @author     Christophe Demko <christophe.demko@univ-lr.fr>
 * @date       2019
 * @copyright  BSD 3-Clause License
 */

#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./ga.h"

int main(void) {
  assert(ga_init());
  assert(ga_init());
  assert(ga_init());
  { /* some code */
  }
  assert(ga_finish());
  assert(ga_finish());
  assert(ga_finish());
  assert(!ga_finish());
  return EXIT_SUCCESS;
}
