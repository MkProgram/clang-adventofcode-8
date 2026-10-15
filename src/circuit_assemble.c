#include "circuit_assemble.h"
#include "coordinates.h"
#include "vec.h"
#include <stdbool.h>
#include <stdio.h>

void process_lines(FILE *file, Vec *v) {

  char *line = NULL;
  size_t linecap = 0;

  while (getline(&line, &linecap, file) != -1) {
    Coordinates c = {0};
    if (coor_from_line(&line, &c) != true) {
      continue;
    }
    vec_push(v, &c);
  }
}

void find_closest_point(Coordinates *loc, size_t current_index, Vec *loc_list,
                        Coordinates *closest) {
  int lowest_distance = 0;
  for (size_t i = 0; i < loc_list->count; ++i) {
    if (current_index == i) {
      continue;
    }
    Coordinates current = {0};
    vec_get(loc_list, i, &current);
    int distance = coor_distance_from(loc, &current);
    if (lowest_distance == 0 || distance < lowest_distance) {
      lowest_distance = distance;
      *closest = current;
    }
  }
  // printf("Closest point to (%d, %d, %d) is (%d, %d, %d) with distance %d\n",
  //       loc->x, loc->y, loc->z, closest->x, closest->y, closest->z,
  //        lowest_distance);
}

size_t assemble_circuit(FILE *file) {
  size_t circuit_sum = 0;

  Vec v = {0};
  vec_init(&v, sizeof(Coordinates *));

  process_lines(file, &v);

  for (size_t i = 0; i < v.count; ++i) {
    Coordinates closest = {0};
    Coordinates current = {0};

    vec_get(&v, i, &current);
    find_closest_point(&current, i, &v, &closest);
  }

  vec_free(&v);

  return circuit_sum;
}
