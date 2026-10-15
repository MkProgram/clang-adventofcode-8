#include "hashmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct get_values {
  size_t key;
  size_t value;
  bool found;
};

bool check_get(const hash_map *map, size_t key, size_t expected_val,
               bool expected_return) {
  size_t actual = 999;
  bool found = map_get(map, key, &actual);

  if (found != expected_return) {
    fprintf(stderr,
            "FAILURE: Expected result of getting a value of key %zu is %s, was "
            "%s\n",
            key, expected_return == true ? "true" : "false",
            found == true ? "true" : "false");
    return false;
  }
  if (found && actual != expected_val) {
    fprintf(stderr, "FAILURE: Expected value of key %zu is %zu, was %zu\n", key,
            expected_val, actual);
    return false;
  }
  if (!found && actual != 999) {
    fprintf(stderr,
            "FAILURE: No value was found but output value was modified anyway. "
            "Key %zu\n",
            key);
    return false;
  }

  return true;
}

size_t test_map_get(void) {
  hash_map map = {0};
  map_init(&map);

  // First three values are used to fill the slots, the ones after are used to
  // test behaviour for calling empty keys
  struct get_values values[] = {{3, 5, true},   {7, 16, true}, {11, 12, true},
                                {12, 0, false}, {2, 0, false}, {0, 0, false}};

  size_t slots[] = {3, 0, 1};

  for (size_t i = 0; i < sizeof slots / sizeof slots[0]; ++i) {
    map.slots[slots[i]] = (hash_slot){values[i].key, values[i].value, true};
  }

  for (size_t i = 0; i < sizeof values / sizeof values[0]; ++i) {
    if (!check_get(&map, values[i].key, values[i].value, values[i].found)) {
      map_free(&map);
      return 1;
    }
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
