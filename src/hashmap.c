#include "hashmap.h"
#include "xmalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void map_init(hash_map *map) {
  size_t capacity = 4;
  hash_slot *slots = checked_calloc(capacity, sizeof(hash_slot));
  *map = (hash_map){slots, capacity, 0};
}

bool map_get(const hash_map *map, size_t key, size_t *out) {

  if (map->capacity == 0) {
    fprintf(stderr, "hash_map has to be initialized first.\n");
    exit(EXIT_FAILURE);
  }
  size_t start = key % map->capacity;

  for (size_t i = 0; i < map->capacity; ++i) {
    size_t index = (start + i) % map->capacity;
    hash_slot current = map->slots[index];

    if (!current.occupied) {
      return false;
    }

    if (current.key == key) {
      *out = current.value;
      return true;
    }
  }
  return false;
}

void map_put(hash_map *map, size_t key, size_t value) {
  hash_slot slot = (hash_slot){key, value, true};

  size_t target = key % map->capacity;

  if (!map->slots[target].occupied) {
    map->slots[target] = slot;
    ++map->count;
    return;
  }

  if (key == map->slots[target].key) {
    map->slots[target].value = value;
  }

  for (size_t i = 0; i < map->capacity; ++i) {
    target = (target + i) % map->capacity;
    if (!map->slots[target].occupied) {
      map->slots[target] = slot;
      ++map->count;
      return;
    }
    if (key == map->slots[target].key) {
      map->slots[target].value = value;
      return;
    }
  }
  assert("No slots available" && true);
}

void map_free(hash_map *map) {
  free(map->slots);
  map->slots = NULL;
  map->capacity = 0;
  map->count = 0;
}
