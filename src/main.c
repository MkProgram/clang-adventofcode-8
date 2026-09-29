
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

  fclose(file);

  return EXIT_SUCCESS;
}
