#include "hashmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct get_values {
  size_t key;
  size_t value;
  bool found;
};
struct put_values {
  size_t key;
  size_t value;
  size_t expected_count;
  size_t expected_capacity;
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

size_t test_put(void) {
  hash_map map = {0};
  map_init_with_size(&map, 4);
  struct put_values values[] = {
      {1, 12, 1, 4},
      {5, 15, 2, 4},
      {9, 19, 3, 8},
      {1, 99, 3, 8},
  };

  for (size_t i = 0; i < sizeof values / sizeof values[0]; ++i) {
    struct put_values current = values[i];
    map_put(&map, current.key, current.value);
    if (map.count != current.expected_count) {
      fprintf(stderr, "FAILURE: Expected a count of %zu, got %zu\n",
              current.expected_count, map.count);
      map_free(&map);
      return 1;
    }
    if (map.capacity != current.expected_capacity) {
      fprintf(stderr,
              "FAILURE: Expected capacity of the map to be %zu, got %zu\n",
              current.expected_capacity, map.capacity);
      map_free(&map);
      return 1;
    }
    if (!check_get(&map, current.key, current.value, true)) {
      map_free(&map);
      return 1;
    }
  }

  struct get_case {
    size_t key;
    size_t value;
  };
  struct get_case cases[] = {{1, 99}, {5, 15}, {9, 19}};

  for (size_t i = 0; i < sizeof cases / sizeof cases[0]; ++i) {
    struct get_case current = cases[i];
    if (!check_get(&map, current.key, current.value, true)) {
      map_free(&map);
      return 1;
    }
  }

  map_free(&map);

  return 0;
}

size_t test_map_get(void) {
  hash_map map = {0};
  map_init_with_size(&map, 4);

  // First three values are used to fill the slots, the ones after are used to
  // test behaviour for calling empty keys
  struct get_values values[] = {
      {3, 5, true},   {7, 16, true}, {11, 12, true},
      {12, 0, false}, {2, 0, false}, {0, 0, false},
  };

  size_t slots[] = {3, 0, 1};

  for (size_t i = 0; i < sizeof slots / sizeof slots[0]; ++i) {
    map.slots[slots[i]] = (hash_slot){values[i].key, values[i].value, true};
  }
  map.count = sizeof slots / sizeof slots[0];

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
  failures += test_put();

  if (failures == 0) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
