#include "circuit_assemble.h"
#include "coordinates.h"
#include "vec.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct COOR_PAIR {
  size_t a;
  size_t b;
  uint64_t distance;
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
bool check_pair_exits(size_t a, size_t b, Vec *found_pairs) {
  if (found_pairs->count == 0) {
    return false;
  }
  for (size_t i = 0; i < found_pairs->count; ++i) {
    struct COOR_PAIR current = {0};
    vec_get(found_pairs, i, &current);
    if ((current.a == a && current.b == b) ||
        (current.a == b && current.b == a)) {
      return true;
    }
  }
  return false;
}

bool find_closest_point(const size_t current_index, const Vec *loc_list,
                        struct COOR_PAIR *out, Vec *found_pairs) {
  uint64_t lowest_distance = 0;
  bool valid = false;
  Coordinates loc = {0};
  vec_get(loc_list, current_index, &loc);
  for (size_t i = current_index; i < loc_list->count; ++i) {
    if (current_index == i || check_pair_exits(current_index, i, found_pairs)) {
      continue;
    }
    Coordinates current = {0};
    vec_get(loc_list, i, &current);
    uint64_t distance = coor_distance_from(&loc, &current);
    if (!valid || distance < lowest_distance) {
      lowest_distance = distance;
      *out = (struct COOR_PAIR){current_index, i, lowest_distance};
      valid = true;
    }
  }

  return valid;
}

bool find_best_pair(Vec *coors, Vec *found_pairs, struct COOR_PAIR *best_pair) {
  bool pair_found = false;
  for (size_t i = 0; i < coors->count; ++i) {
    Coordinates current = {0};
    struct COOR_PAIR current_pair = {0};

    vec_get(coors, i, &current);
    if (find_closest_point(i, coors, &current_pair, found_pairs) == false) {
      continue;
    }
    if (!pair_found || current_pair.distance < best_pair->distance) {
      *best_pair = current_pair;
      pair_found = true;
    }
  }
  return pair_found;
}

void connect_boxes(Vec *v, size_t count) {
  Vec found_pairs = {0};
  vec_init(&found_pairs, sizeof(struct COOR_PAIR));
  struct COOR_PAIR best_pair = {0};

  for (size_t i = 0; i < count; ++i) {
    if (find_best_pair(v, &found_pairs, &best_pair)) {
      vec_push(&found_pairs, &best_pair);
      Coordinates best_a = {0};
      Coordinates best_b = {0};
      vec_get(v, best_pair.a, &best_a);
      vec_get(v, best_pair.b, &best_b);
      printf(
          "The best pair: (%d, %d, %d), (%d, %d, %d) with distance of %" PRIu64
          "\n",
          best_a.x, best_a.y, best_a.z, best_b.x, best_b.y, best_b.z,
          best_pair.distance);
    }
  }

  vec_free(&found_pairs);
}

size_t assemble_circuit(FILE *file, size_t count) {
  size_t circuit_sum = 0;

  Vec v = {0};
  vec_init(&v, sizeof(Coordinates));

  process_lines(file, &v);
  connect_boxes(&v, count);

  vec_free(&v);

  return circuit_sum;
}
