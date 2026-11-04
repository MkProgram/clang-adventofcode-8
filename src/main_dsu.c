#include "circuit_assemble_dsu.h"
#include <inttypes.h>
#include <stdlib.h>

int main(void) {
  const char *floc = "input.txt";
  puts("Lets connect some circuits.");
  FILE *file = NULL;
  file = fopen(floc, "r");
  if (file == NULL) {
    fprintf(stderr, "File could not be opened: %s\n", floc);
    return EXIT_FAILURE;
  }
  CircuitResults result = assemble_circuit_dsu(file, 1000);
  printf("Combined circuits = %zu\n", result.part1);
  printf("Result for part 2 is: %" PRIu64 "\n", result.part2);

  fclose(file);

  return EXIT_SUCCESS;
}
