#ifndef COORDINATES_H
#define COORDINATES_H
#include <stdbool.h>
#include <stddef.h>
typedef struct {
  int x;
  int y;
  int z;
} Coordinates;

bool coor_from_line(char **line, Coordinates *c);
int coor_distance_from(Coordinates *loc, Coordinates *des);
bool coor_is_equals(Coordinates *a, Coordinates *b);

#endif // !COORDINATES_H
