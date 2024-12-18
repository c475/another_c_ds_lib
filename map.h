#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include "error.h"
#include "array.h"
#include "string.h"

typedef int (*map_cmp_t)(void *a, void *b);
typedef uint64_t (*map_hash_t)(void *key);
typedef void (*destructor_t)(void *);

typedef struct map {
    array *buckets;
    map_cmp_t cmp_func;
    map_hash_t hash_func;
    void (*key_destructor)(void *);
    void (*value_destructor)(void *);
    size_t size;
} map;

/* could use container_of() to avoid the destructor fields */
typedef struct map_node {
    void *key;
    void *value;
    uint64_t hash;
    map *container;
} map_node;

map *map_new(map_cmp_t cmp, map_hash_t hash, destructor_t key_dest, destructor_t val_dest);
void map_insert(map *m, void *key, void *value);
void *map_get(map *m, void *key);
bool map_delete(map *m, void *key);
bool map_contains(map *m, void *key);
void map_free(map *m);
int map_default_cmp(void *a, void *b);
uint64_t map_default_hash(void *key);

/* Disgusting map foreach macro */
#define foreach_map(M, E) size_t LINE_NAME(i) = 0; size_t LINE_NAME(j) = 0; array *LINE_NAME(bucket); map_node *E; \
for ( \
    LINE_NAME(bucket) = arr_getornull(M->buckets, LINE_NAME(i)); \
    LINE_NAME(i) < M->buckets->cap; \
    LINE_NAME(i) ++, LINE_NAME(bucket) = arr_getornull(M->buckets, LINE_NAME(i)) \
) \
    for ( \
        E = *(LINE_NAME(bucket) -> contents), LINE_NAME(j) = 0; \
        LINE_NAME(j) < arr_len(LINE_NAME(bucket)); \
        LINE_NAME(j) ++, E = *(LINE_NAME(bucket) -> contents + LINE_NAME(j)) \
    )
