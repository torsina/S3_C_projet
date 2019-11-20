#ifndef GENETIC_GENERATOR_STRUCT_
#define GENETIC_GENERATOR_STRUCT_

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

#endif  // GENETIC_GENERATOR_STRUCT_
