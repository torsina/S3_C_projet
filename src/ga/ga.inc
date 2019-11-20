#ifndef SRC_GA_GA_INC_
#define SRC_GA_GA_INC_

struct _GeneticGenerator {
  unsigned int size;
  unsigned int* cardinalities;
};

struct _Individual {
  unsigned int *genes;
};

struct _Population {
  unsigned int size;
  const struct _GeneticGenerator* generator;
  struct _Individual** individuals;
};

#endif  // SRC_GA_GA_INC_
