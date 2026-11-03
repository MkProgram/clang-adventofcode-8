#include "circuit_assemble.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t test_example_file(void) {
  const char *floc = "tests/example.txt";
  const size_t expected = 40;
  const uint64_t expected_part2 = 25272;

  FILE *file = NULL;
  file = fopen(floc, "r");

  if (file == NULL) {
    fprintf(stderr,
            "FAILURE: Example File: Could not open example file \"%s\".\n",
            floc);
    return 1;
  }
  uint64_t part2 = 0;
  size_t actual = assemble_circuit(file, 10, &part2);
  if (actual != expected) {
    fprintf(stderr, "FAILURE: Example File: Expected %zu, got %zu.\n", expected,
            actual);
    return 1;
  }
  if (part2 != expected_part2) {
    fprintf(stderr,
            "FAILURE: Example File: Expected %" PRIu64 ", got %" PRIu64 "\n",
            expected_part2, part2);
  }
  fclose(file);

  return 0;
}

int main(void) {
  size_t failures = 0;

  failures += test_example_file();
  if (failures == 0) {
    printf("Tests completed\n");
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
