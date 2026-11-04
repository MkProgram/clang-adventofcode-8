#include "circuit_assemble_dsu.h"
#include "coordinates.h"
#include "vec.h"
#include "xmalloc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Edge {
  size_t a;
  size_t b;
  uint64_t distance;
};

struct DisjointSet {
  size_t *parent;
  size_t *component_size;
  size_t component_count;
  size_t point_count;
};

static struct DisjointSet disjoint_set_create(size_t count) {
  struct DisjointSet set = {0};

  if (count == 0) {
    return set;
  }

  set.parent = checked_calloc(count, sizeof *set.parent);
  set.component_size = checked_calloc(count, sizeof *set.component_size);
  set.component_count = count;
  set.point_count = count;

  for (size_t i = 0; i < count; ++i) {
    set.parent[i] = i;
    set.component_size[i] = 1;
  }

  return set;
}

static void disjoint_set_free(struct DisjointSet *set) {
  free(set->parent);
  free(set->component_size);
  *set = (struct DisjointSet){0};
}

static size_t find_root(struct DisjointSet *set, size_t point) {
  size_t root = point;

  while (set->parent[root] != root) {
    root = set->parent[root];
  }

  while (set->parent[point] != point) {
    size_t next = set->parent[point];
    set->parent[point] = root;
    point = next;
  }

  return root;
}

static bool disjoint_set_union(struct DisjointSet *set, size_t point_a,
                               size_t point_b) {
  size_t root_a = find_root(set, point_a);
  size_t root_b = find_root(set, point_b);

  if (root_a == root_b) {
    return false;
  }

  if (set->component_size[root_a] < set->component_size[root_b]) {
    size_t temporary = root_a;
    root_a = root_b;
    root_b = temporary;
  }

  set->parent[root_b] = root_a;
  set->component_size[root_a] += set->component_size[root_b];
  --set->component_count;

  return true;
}

static int compare_edges(const void *lhs, const void *rhs) {
  const struct Edge *a = lhs;
  const struct Edge *b = rhs;

  if (a->distance < b->distance)
    return -1;
  if (a->distance > b->distance)
    return 1;

  // Make equal-distance ordering predictable.
  if (a->a < b->a)
    return -1;
  if (a->a > b->a)
    return 1;
  if (a->b < b->b)
    return -1;
  if (a->b > b->b)
    return 1;

  return 0;
}

static struct Edge *build_sorted_edges(const Vec *points,
                                       size_t *edge_count_out) {
  const size_t point_count = points->count;

  if (point_count < 2) {
    *edge_count_out = 0;
    return NULL;
  }

  // Calculate point_count * (point_count -1) / 2,
  // dividing first to reduce overflow risk
  size_t left = point_count;
  size_t right = point_count - 1;

  if (left % 2 == 0) {
    left /= 2;
  } else {
    right /= 2;
  }

  if (right > SIZE_MAX / left) {
    fprintf(stderr, "Too many points to build edge list\n");
    abort();
  }

  const size_t edge_count = left * right;
  struct Edge *edges = checked_calloc(edge_count, sizeof *edges);
  size_t next = 0;

  for (size_t i = 0; i < point_count; ++i) {
    Coordinates a = {0};
    vec_get(points, i, &a);

    for (size_t j = i + 1; j < point_count; ++j) {
      Coordinates b = {0};
      vec_get(points, j, &b);

      edges[next++] =
          (struct Edge){.a = i, .b = j, .distance = coor_distance_from(&a, &b)};
    }
  }

  qsort(edges, edge_count, sizeof *edges, compare_edges);
  *edge_count_out = edge_count;
  return edges;
}

static size_t largest_three_product(const struct DisjointSet *set) {
  size_t largest[3] = {0};

  for (size_t i = 0; i < set->point_count; ++i) {
    if (set->parent[i] != i) {
      continue;
    }

    size_t size = set->component_size[i];

    if (size > largest[0]) {
      largest[2] = largest[1];
      largest[1] = largest[0];
      largest[0] = size;
    } else if (size > largest[1]) {
      largest[2] = largest[1];
      largest[1] = size;
    } else if (size > largest[2]) {
      largest[2] = size;
    }
  }

  return largest[0] * largest[1] * largest[2];
}

static CircuitResults process_sorted_edges(const Vec *points,
                                           const struct Edge *edges,
                                           size_t edge_count,
                                           size_t connection_count) {
  CircuitResults results = {0};
  struct DisjointSet set = disjoint_set_create(points->count);

  const size_t part1_after =
      connection_count < edge_count ? connection_count : edge_count;
  bool part1_found = (part1_after == 0);

  if (part1_found) {
    results.part1 = largest_three_product(&set);
  }

  for (size_t i = 0; i < edge_count; ++i) {
    const struct Edge *edge = &edges[i];
    bool merged = disjoint_set_union(&set, edge->a, edge->b);

    // Every candidate edge counts toward part one, even a redundant one.
    if (!part1_found && i + 1 == part1_after) {
      results.part1 = largest_three_product(&set);
      part1_found = true;
    }

    // Part two uses the edge that successfully joins the final components.
    if (merged && set.component_count == 1) {
      Coordinates a = {0};
      Coordinates b = {0};
      vec_get(points, edge->a, &a);
      vec_get(points, edge->b, &b);

      results.part2 = (uint64_t)a.x * (uint64_t)b.x;
      results.part2_found = true;
    }

    if (part1_found && results.part2_found) {
      break;
    }
  }

  disjoint_set_free(&set);
  return results;
}

CircuitResults assemble_circuit_dsu(FILE *file, size_t connection_count) {
  Vec points = {0};
  vec_init(&points, sizeof(Coordinates));

  char *line = NULL;
  size_t line_capacity = 0;

  while (getline(&line, &line_capacity, file) != -1) {
    Coordinates point = {0};
    if (!coor_from_line(&line, &point)) {
      continue;
    }

    vec_push(&points, &point);
  }
  if (line != NULL) {
    free(line);
  }

  size_t edge_count = 0;
  struct Edge *edges = build_sorted_edges(&points, &edge_count);

  CircuitResults results =
      process_sorted_edges(&points, edges, edge_count, connection_count);
  free(edges);
  vec_free(&points);

  return results;
}
