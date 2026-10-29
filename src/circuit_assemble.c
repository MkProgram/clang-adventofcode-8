#include "circuit_assemble.h"
#include "coordinates.h"
#include "vec.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct COOR_PAIR {
  size_t a;
  size_t b;
  int distance;
};

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

  if (line != NULL) {
    free(line);
  }
}

bool find_closest_point(const size_t current_index, const Vec *loc_list,
                        struct COOR_PAIR *out) {
  int lowest_distance = 0;
  bool valid = false;
  Coordinates loc = {0};
  vec_get(loc_list, current_index, &loc);
  for (size_t i = current_index; i < loc_list->count; ++i) {
    if (current_index == i) {
      continue;
    }
    Coordinates current = {0};
    vec_get(loc_list, i, &current);
    int distance = coor_distance_from(&loc, &current);
    if (!valid || distance < lowest_distance) {
      lowest_distance = distance;
      *out = (struct COOR_PAIR){current_index, i, lowest_distance};
      valid = true;
    }
  }

  return valid;
}

void connect_boxes(Vec *v) {
  struct COOR_PAIR best_pair = {0};
  bool pair_found = false;
  for (size_t i = 0; i < v->count; ++i) {
    Coordinates current = {0};
    struct COOR_PAIR current_pair = {0};

    vec_get(v, i, &current);
    printf("Coordinates: %d, %d, %d\n", current.x, current.y, current.z);
    if (find_closest_point(i, v, &current_pair) == false) {
      continue;
    }
    if (!pair_found || current_pair.distance < best_pair.distance) {
      best_pair = current_pair;
      pair_found = true;
    }
  }
  if (pair_found) {
    Coordinates best_a = {0};
    Coordinates best_b = {0};
    vec_get(v, best_pair.a, &best_a);
    vec_get(v, best_pair.b, &best_b);
    printf("The best pair: (%d, %d, %d), (%d, %d, %d) with distance of %d\n",
           best_a.x, best_a.y, best_a.z, best_b.x, best_b.y, best_b.z,
           best_pair.distance);
  }
}

size_t assemble_circuit(FILE *file) {
  size_t circuit_sum = 0;

  Vec v = {0};
  vec_init(&v, sizeof(Coordinates));

  process_lines(file, &v);

  vec_free(&v);

  return circuit_sum;
}
