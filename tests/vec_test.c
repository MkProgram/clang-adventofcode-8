#include "vec.h"
#include <stdio.h>
#include <stdlib.h>

static const char *char_val = "This string";

typedef struct {
  char *name;
  void *value;
} VEC_PUSH_CASE;

const static VEC_PUSH_CASE cases[] = {{"String", &char_val}};

size_t test_vec(void) {
  size_t failures = 0;
  for (size_t i = 0; i < sizeof cases / sizeof cases[0]; ++i) {
    VEC_PUSH_CASE current = cases[i];
    Vec v = {0};

    vec_init(&v, sizeof current.value);
    vec_push(&v, current.value);
    char *actual = NULL;
    vec_get(&v, 0, &actual);
    if (actual == NULL || actual != current.value) {
      fprintf(stderr, "FAILURE: %s: Expected value does not match the return.",
              current.name);
      ++failures;
    }
  }
  return failures;
}

int main(void) {
  size_t failures = 0;

  failures += test_vec();
  if (failures == 0) {
    return EXIT_FAILURE;
  }

  return EXIT_FAILURE;
}
