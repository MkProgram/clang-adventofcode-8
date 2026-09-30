#include "vec.h"
#include "xmalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vec_init(Vec *v, size_t elem_size) { *v = (Vec){NULL, 0, 0, elem_size}; }

void vec_push(Vec *v, const void *value) {
  assert(v->elem_size != 0 &&
         "Vec should be initialized first, elem_size is not allowed to be 0.");
  if (v->count >= v->capacity) {
    size_t capacity = v->capacity == 0 ? 4 : v->capacity * 2;
    void *grow = checked_realloc(v->data, capacity * v->elem_size);
    v->data = grow;
    v->capacity = capacity;
  }
  char *base = (char *)v->data;
  char *slot = base + (v->count * v->elem_size);
  memcpy(slot, value, v->elem_size);
  ++v->count;
}

void vec_get(const Vec *v, size_t index, void *out) {
  if (v->count <= index) {
    fprintf(stderr,
            "Array out of bounds. Tried to access %zu with a length of %zu\n",
            index, v->count);
    exit(EXIT_FAILURE);
  }
  char *base = (char *)v->data;
  char *slot = base + (index * v->elem_size);
  memcpy(out, slot, v->elem_size);
}

void vec_free(Vec *v) {
  free(v->data);
  v->data = NULL;
  v->count = 0;
  v->capacity = 0;
  v->elem_size = 0;
}
