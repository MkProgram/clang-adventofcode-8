#include "coordinates.h"
#include <stdbool.h>
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

int coor_distance_from(Coordinates *loc, Coordinates *des) {
  // Actual distance requires a square root, since we only compare distance, it
  // is not necessary
  return ((loc->x - des->x) * (loc->x - des->x)) +
         ((loc->y - des->y) * (loc->y - des->y)) +
         ((loc->z - des->z) * (loc->z - des->z));
}

bool coor_is_equals(Coordinates *a, Coordinates *b) {
  return a->x == b->x && a->y == b->y && a->z == b->z;
}
