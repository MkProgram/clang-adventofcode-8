#ifndef COORDINATES_H
#define COORDINATES_H
#include <stddef.h>
typedef struct {
  int x;
  int y;
  int z;
} Coordinates;

void from_line(char **line, Coordinates *c);
int distance_from(Coordinates *loc, Coordinates *des);

#endif // !COORDINATES_H
