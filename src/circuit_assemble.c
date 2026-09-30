#include "circuit_assemble.h"
#include <stdio.h>

size_t assemble_circuit(FILE *file) {
  size_t circuit_sum = 0;

  char *line = NULL;
  size_t linecap = 0;

  while (getline(&line, &linecap, file) != -1) {
  }

  return circuit_sum;
}
