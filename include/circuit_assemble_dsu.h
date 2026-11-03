#ifndef CIRCUIT_ASSEMBLE_DSU_H
#define CIRCUIT_ASSEMBLE_DSU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
typedef struct {
  size_t part1;
  uint64_t part2;
  bool part2_found;
} CircuitResults;

CircuitResults assemble_circuit_dsu(FILE *file, size_t connection_count);

#endif // !CIRCUIT_ASSEMBLE_DSU_H
