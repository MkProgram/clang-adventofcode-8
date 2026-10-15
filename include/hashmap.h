#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
typedef struct {
  size_t key;
  size_t value;
  bool occupied;
} hash_slot;

typedef struct {
  hash_slot *slots;
  size_t capacity;
  size_t count;
} hash_map;

void map_init(hash_map *map);
void map_put(hash_map *map, size_t key, size_t value);
bool map_get(const hash_map *map, size_t key, size_t *out);
void map_free(hash_map *map);

#endif // !HASHMAP_H
