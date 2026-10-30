#include "coordinates.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool coor_from_line(char **line, Coordinates *c) {
  int x = 0;
  int y = 0;
  int z = 0;
  if (sscanf(*line, "%d,%d,%d", &x, &y, &z) != 3) {
    fprintf(stderr, "Line \"%s\" is malformed and cannot be processed.", *line);
    return false;
  }
  *c = (Coordinates){x, y, z};
  return true;
}

uint64_t coor_distance_from(Coordinates *loc, Coordinates *des) {
  // Actual distance requires a square root, since we only compare distance, it
  // is not necessary
  const int64_t dx = (int64_t)loc->x - des->x;
  const int64_t dy = (int64_t)loc->y - des->y;
  const int64_t dz = (int64_t)loc->z - des->z;
  return (uint64_t)((dx * dx) + (dy * dy) + (dz * dz));
}

bool coor_is_equals(Coordinates *a, Coordinates *b) {
  return a->x == b->x && a->y == b->y && a->z == b->z;
}
