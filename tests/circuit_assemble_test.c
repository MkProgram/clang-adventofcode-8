#include "circuit_assemble.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

size_t test_example_file(void) {
  const char *floc = "tests/example.txt";
  const size_t expected = 40;

  FILE *file = NULL;
  file = fopen(floc, "r");

  if (file == NULL) {
    fprintf(stderr,
            "FAILURE: Example File: Could not open example file \"%s\".", floc);
    return 1;
  }

  size_t actual = assemble_circuit(file);
  if (actual != expected) {
    fprintf(stderr, "FAILURE: Example File: Expected %zu, got %zu.", expected,
            actual);
    return 1;
  }
  fclose(file);

  return 0;
}

int main(void) {
  size_t failures = 0;

  failures += test_example_file();
  if (failures == 0) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
