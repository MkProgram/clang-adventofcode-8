#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>

static void throw_out_of_memory(void) {
  fprintf(stderr, "Out of memory\n");
  exit(EXIT_FAILURE);
}

void *checked_realloc(void *ptr, size_t size) {
  void *result = realloc(ptr, size);
  if (result == NULL) {
    throw_out_of_memory();
  }
  return result;
}

void *checked_malloc(size_t size) {
  void *result = malloc(size);
  if (result == NULL) {
    throw_out_of_memory();
  }
  return result;
}

void *checked_calloc(size_t num, size_t size) {
  void *result = calloc(num, size);
  if (result == NULL) {
    throw_out_of_memory();
  }
  return result;
}
