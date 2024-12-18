#include "map.h"

/************ Map node stuff ************/

map_node *map_node_new(void *key, void *value, uint64_t hash, map *m)
{
    map_node *node;

    node = calloc(1, sizeof(map_node));
    if (!node)
        err_exit("calloc() error");

    node->key = key;
    node->value = value;
    node->hash = hash;
    node->container = m;
    return node;
}

void map_node_free(void *n)
{
    map_node *node = n;

    if (node->container->key_destructor)
        node->container->key_destructor(node->key);
    if (node->container->value_destructor)
        node->container->value_destructor(node->value);

    free(node);
}


/************ Actual map stuff ************/

map *map_new(map_cmp_t cmp_func, map_hash_t hash_func, destructor_t key_dest, destructor_t val_dest)
{
    map *new_map;
    array *buckets;

    new_map = calloc(1, sizeof(map));
    if (!new_map)
        err_exit("map creation error");

    buckets = arr_new(arr_free);

    new_map->buckets = buckets;
    new_map->cmp_func = cmp_func ? cmp_func : map_default_cmp;
    new_map->hash_func = hash_func ? hash_func : map_default_hash;
    new_map->key_destructor = key_dest;
    new_map->value_destructor = val_dest;
    new_map->size = 0;

    // maybe wait until it's needed instead, idk
    for (size_t i = 0; i < arr_cap(buckets); i++) {
        array *tmp = arr_new(map_node_free);
        arr_push(buckets, tmp);
    }

    return new_map;
}

// double the number of buckets and rehash everything
static void map_resize(map *m)
{
    size_t i;
    array *new_buckets;

    new_buckets = arr_new(arr_free);
    for (i = 0; i < arr_cap(m->buckets) * 2; i++) {
        array *tmp = arr_new(map_node_free);
        arr_push(new_buckets, tmp);
    }

    for (i = 0; i < arr_len(m->buckets); i++) {
        array *old_bucket = arr_get(m->buckets, i);
        while (arr_len(old_bucket) > 0) {
            map_node *node = arr_pop(old_bucket);
            array *new_bucket = arr_get(new_buckets, node->hash % arr_len(new_buckets));
            arr_push(new_bucket, node);
        }
    }
    arr_free(m->buckets);

    m->buckets = new_buckets;
}

void map_insert(map *m, void *key, void *value)
{
    uint64_t hash;
    array *bucket;
    map_node *node;

    if (m->size >= ARRAY_DEFAULT_CAPACITY * arr_cap(m->buckets) * 2/3)
        map_resize(m);

    hash = m->hash_func(key);
    node = map_node_new(key, value, hash, m);
    bucket = arr_get(m->buckets, hash % arr_cap(m->buckets));
    arr_push(bucket, node);

    m->size++;
}

static inline map_node *map_get_node(map *m, void *key)
{
    uint64_t hash;
    array *bucket;

    hash = m->hash_func(key);
    bucket = arr_get(m->buckets, hash % arr_cap(m->buckets));
    foreach(bucket, map_node *, node) {
        if (node->hash == hash && m->cmp_func(key, node->key) == 0)
            return node;
    }

    return NULL;
}

void *map_get(map *m, void *key)
{
    map_node *node;

    node = map_get_node(m, key);
    return node ? node->value : NULL;
}

bool map_delete(map *m, void *key)
{
    map_node *result;

    result = map_get_node(m, key);
    if (result) {
        result->value = NULL;
        return true;
    }

    return false;
}

bool map_contains(map *m, void *key)
{
    map_node *node;

    node = map_get_node(m, key);
    return node != NULL;
}

void map_free(map *m)
{
    arr_free(m->buckets);
    free(m);
}

int map_default_cmp(void *a, void *b)
{
    return a == b;
}

// fnv1a
uint64_t map_default_hash(void *key)
{
    uint64_t hash = 0xcbf29ce484222325;
    uint64_t val = (uint64_t)key;

    while (val) {
        hash ^= val & 0xff;
        hash *= 0x100000001b3;
        val >>= 8;
    }

    return hash;
}
