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

void map_init_with_size(hash_map *map, size_t size) {
  hash_slot *slots = checked_calloc(size, sizeof(hash_slot));
  *map = (hash_map){slots, size, 0};
}

static void grow(hash_map *map) {
  hash_slot *grown = checked_calloc(map->capacity * 2, sizeof(hash_slot));
  hash_map grown_map = {0};
  map_init(&grown_map);
  for (size_t i = 0; i < map->capacity; ++i) {
    hash_slot current = map->slots[i];
    if (!current.occupied) {
      continue;
    }
    map_put(&grown_map, current.key, current.value);
  }
  map_free(map);
  map = &grown_map;
}

bool map_get(const hash_map *map, size_t key, size_t *out) {
  assert("Map has to be initialized first" && map->capacity != 0);
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
  assert("Map has to be initialized first" && map->capacity != 0);
  if (map->count >= map->capacity) {
    assert("No slots available" && false);
  }
  if (map->count * 10 >= map->capacity * 7) {
  }

  hash_slot slot = (hash_slot){key, value, true};
  size_t start = key % map->capacity;

  for (size_t i = 0; i < map->capacity; ++i) {
    size_t index = (start + i) % map->capacity;
    if (!map->slots[index].occupied) {
      map->slots[index] = slot;
      ++map->count;
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
