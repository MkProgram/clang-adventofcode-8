#include "hashmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

size_t test_map_get(void) {
  hash_map map = {0};
  map_init(&map);

  size_t v1 = 5;
  size_t v2 = 16;
  size_t v3 = 12;

  map.slots[3] = (hash_slot){3, v1, true};
  map.slots[0] = (hash_slot){7, v2, true};
  map.slots[1] = (hash_slot){11, v3, true};

  size_t t1 = 0;
  size_t t2 = 0;
  size_t t3 = 0;

  bool valid = map_get(&map, 3, &t1);

  if (!valid) {
    fprintf(stderr, "FAILURE: Failed to find key %d\n", 5);
    return 1;
  }

  if (t1 != v1) {
    fprintf(stderr, "FAILURE: Expected %zu, got %zu\n", v1, t1);
    return 1;
  }

  valid = map_get(&map, 7, &t2);

  if (!valid) {
    fprintf(stderr, "FAILURE: Failed to find key %d\n", 9);
    return 1;
  }

  if (t2 != v2) {
    fprintf(stderr, "FAILURE: Expected %zu, got %zu\n", v2, t2);
    return 1;
  }

  valid = map_get(&map, 11, &t3);
  if (!valid) {
    fprintf(stderr, "FAILURE: Failed to find key %d\n", 9);
    return 1;
  }
  if (t3 != v3) {
    fprintf(stderr, "FAILURE: Expected %zu, got %zu\n", v3, t3);
    return 1;
  }
  size_t t4 = 0;
  valid = map_get(&map, 12, &t4);
  if (valid) {
    fprintf(stderr, "FAILURE: Found a key, that is not set in the map %d\n",
            12);
    return 1;
  }

  map_free(&map);
  return 0;
}

int main(void) {
  size_t failures = 0;

  failures += test_map_get();

  if (failures == 0) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
