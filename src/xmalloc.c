#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>

void *checked_realloc(void *ptr, size_t size) {
  void *result = realloc(ptr, size);
  if (result == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }
  return result;
}
