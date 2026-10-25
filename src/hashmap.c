#include "hashmap.h"
#include "xmalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void map_init_with_size(hash_map *map, size_t size) {
  if (size == 0) {
    fprintf(stderr, "Map cannot be initialized with size of 0\n");
    abort();
  }
  if (map == NULL) {
    fprintf(stderr, "Map cannot be null\n");
    abort();
  }
  hash_slot *slots = checked_calloc(size, sizeof(hash_slot));
  *map = (hash_map){slots, size, 0};
}

void map_init(hash_map *map) {
  size_t size = 100;
  map_init_with_size(map, size);
}

static void grow(hash_map *map) {
  const size_t capacity = map->capacity * 2;
  hash_map grown_map = {0};
  map_init_with_size(&grown_map, capacity);
  for (size_t i = 0; i < map->capacity; ++i) {
    hash_slot current = map->slots[i];
    if (!current.occupied) {
      continue;
    }
    map_put(&grown_map, current.key, current.value);
  }
  map_free(map);
  *map = grown_map;
}

static void check_initialized(const hash_map *map) {
  if (map == NULL || map->capacity == 0 || map->slots == NULL) {
    fprintf(stderr, "Map has to be initialized first\n");
    abort();
  }
}

bool map_get(const hash_map *map, size_t key, size_t *out) {
  check_initialized(map);
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
  check_initialized(map);
  hash_slot slot = (hash_slot){key, value, true};
  size_t start = key % map->capacity;

  for (size_t i = 0; i < map->capacity; ++i) {
    size_t index = (start + i) % map->capacity;
    if (!map->slots[index].occupied) {
      map->slots[index] = slot;
      ++map->count;
      if (map->count * 10 >= map->capacity * 7) {
        grow(map);
      }
      return;
    }
    if (key == map->slots[index].key) {
      map->slots[index].value = value;
      return;
    }
  }
  assert("No slots available" && false);
}

void map_free(hash_map *map) {
  free(map->slots);
  map->slots = NULL;
  map->capacity = 0;
  map->count = 0;
}
