#include "circuit_assemble.h"
#include "coordinates.h"
#include "hashmap.h"
#include "vec.h"
#include "xmalloc.h"
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

static int compare_pairs(const void *lhs, const void *rhs) {
  const struct COOR_PAIR *a = lhs;
  const struct COOR_PAIR *b = rhs;

  if (a->distance < b->distance)
    return -1;
  if (a->distance > b->distance)
    return 1;
  return 0;
}

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

bool find_best_pair(const Vec *coors, Vec *found_pairs,
                    struct COOR_PAIR *best_pair) {
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

size_t calculate_circuits(hash_map *map, size_t point_count) {
  size_t *group_sizes = checked_calloc(point_count, sizeof *group_sizes);

  for (size_t point = 0; point < point_count; ++point) {
    size_t group_id = 0;
    if (!map_get(map, point, &group_id)) {
      fprintf(stderr, "Missing point in group map\n");
      abort();
    }
    ++group_sizes[group_id];
  }
  size_t *largest_group_count = checked_calloc(3, sizeof *largest_group_count);

  for (size_t i = 0; i < point_count; ++i) {
    size_t current = group_sizes[i];

    if (current > largest_group_count[0]) {
      largest_group_count[2] = largest_group_count[1];
      largest_group_count[1] = largest_group_count[0];
      largest_group_count[0] = current;
    } else if (current > largest_group_count[1]) {
      largest_group_count[2] = largest_group_count[1];
      largest_group_count[1] = current;
    } else if (current > largest_group_count[2]) {
      largest_group_count[2] = current;
    }
  }
  size_t calc =
      largest_group_count[0] * largest_group_count[1] * largest_group_count[2];

  free(largest_group_count);
  free(group_sizes);
  return calc;
}

bool assign_group(hash_map *map, size_t point_count,
                  const struct COOR_PAIR *best_pair) {
  size_t group_a = 0;
  size_t group_b = 0;
  if (!map_get(map, best_pair->a, &group_a) ||
      !map_get(map, best_pair->b, &group_b)) {
    fprintf(stderr, "Cannot Assign group on pair (%zu, %zu)", best_pair->a,
            best_pair->b);
    abort();
  }
  if (group_a == group_b) {
    return false;
  }

  for (size_t point = 0; point < point_count; ++point) {
    size_t group = 0;

    if (!map_get(map, point, &group)) {
      fprintf(stderr, "Missing point in group map\n");
      abort();
    }

    if (group == group_b) {
      map_put(map, point, group_a);
    }
  }
  return true;
}

void connect_boxes(const Vec *v, const size_t count, size_t *part1,
                   uint64_t *part2) {
  hash_map map = {0};
  map_init(&map);

  for (size_t i = 0; i < v->count; ++i) {
    map_put(&map, i, i);
  }
  size_t point_count = v->count;

  size_t left = point_count;
  size_t right = point_count > 0 ? point_count - 1 : 0;
  if (left % 2 == 0) {
    left /= 2;
  } else {
    right /= 2;
  }

  if (left != 0 && right > SIZE_MAX / left) {
    fprintf(stderr, "Too many points to enumerate pairs\n");
    abort();
  }
  size_t pair_count = left * right;

  struct COOR_PAIR *pairs = checked_calloc(pair_count, sizeof *pairs);

  size_t next = 0;
  for (size_t i = 0; i < point_count; ++i) {
    Coordinates a = {0};
    vec_get(v, i, &a);

    for (size_t j = i + 1; j < point_count; ++j) {
      Coordinates b = {0};
      vec_get(v, j, &b);

      pairs[next++] = (struct COOR_PAIR){i, j, coor_distance_from(&a, &b)};
    }
  }

  qsort(pairs, pair_count, sizeof *pairs, compare_pairs);
  size_t group_count = point_count;
  for (size_t i = 0; i < pair_count && group_count > 1; ++i) {
    bool merged = assign_group(&map, point_count, &pairs[i]);

    if (i + 1 == count) {
      *part1 = calculate_circuits(&map, point_count);
    }

    if (!merged) {
      continue;
    }

    --group_count;

    if (group_count == 1) {
      Coordinates a = {0};
      Coordinates b = {0};
      vec_get(v, pairs[i].a, &a);
      vec_get(v, pairs[i].b, &b);

      *part2 = (uint64_t)a.x * (uint64_t)b.x;
    }
  }

  free(pairs);
  map_free(&map);
}

size_t assemble_circuit(FILE *file, size_t count, uint64_t *part2) {
  size_t part1 = 0;
  Vec v = {0};
  vec_init(&v, sizeof(Coordinates));

  process_lines(file, &v);
  connect_boxes(&v, count, &part1, part2);

  vec_free(&v);

  return part1;
}
