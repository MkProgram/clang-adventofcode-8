#include "circuit_assemble.h"
#include <inttypes.h>
#include <stdio.h>
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
  uint64_t part2 = 0;
  size_t result = assemble_circuit(file, 1000, &part2);
  printf("Combined circuits = %zu\n", result);
  printf("Result for part 2 is: %" PRIu64 "\n", part2);

  fclose(file);

  return EXIT_SUCCESS;
}
