#ifndef VEC_H
#define VEC_H
#include <stddef.h>
typedef struct {
  void *data;
  size_t count;
  size_t capacity;
  size_t elem_size;
} Vec;

void vec_init(Vec *v, size_t elem_size);
void vec_push(Vec *v, const void *value);
void vec_get(const Vec *v, size_t index, void *out);
void vec_free(Vec *v);

#endif // !VEC_H
