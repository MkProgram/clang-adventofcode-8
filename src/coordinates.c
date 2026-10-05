#include "coordinates.h"
#include <stdio.h>

void from_line(char **line, Coordinates *c) {
  int x = 0;
  int y = 0;
  int z = 0;
  if (sscanf(*line, "%d,%d,%d", &x, &y, &z) != 3) {
    fprintf(stderr, "Line \"%s\" is malformed and cannot be processed.", *line);
    return;
  }
  *c = (Coordinates){x, y, z};
}

int distance_from(Coordinates *loc, Coordinates *des) {
  // Actual distance requires a square root, but we don't need that for our
  // purposes, so we can just return the squared distance.
  return ((loc->x - des->x) * (loc->x - des->x)) +
         ((loc->y - des->y) * (loc->y - des->y)) +
         ((loc->z - des->z) * (loc->z - des->z));
}
