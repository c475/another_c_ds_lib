#include "array.h"
#include <stdlib.h>

array *arr_new(void (*destructor)(void *))
{
    array *arr = calloc(1, sizeof(array));
    if (!arr)
        err_exit("calloc error (darray)");

    arr->destructor = destructor;
    arr->cap = ARRAY_DEFAULT_CAPACITY;
    arr->contents = calloc(ARRAY_DEFAULT_CAPACITY, sizeof(void *));
    if (!arr->contents)
        err_exit("calloc error (contents)");

    return arr;
}

void *arr_get(array *arr, size_t i)
{
    if (arr->start + i >= arr->end)
        err_exit("index 'i' out of bounds");

    return arr->contents[arr->start + i];
}

void *arr_getornull(array *arr, size_t i)
{
    if (arr->start + i >= arr->end)
        return NULL;

    return arr->contents[arr->start + i];
}

/* Take ownership of the element so destructor isn't called on it in arr_free() */
void *arr_take(array *arr, size_t i)
{
    void *element;

    if (arr->start + i >= arr->end)
        err_exit("index 'i' out of bounds");

    element = arr->contents[arr->start + i];
    arr->contents[arr->start + i] = NULL;
    return element;
}

void arr_set(array *arr, void *element, size_t i)
{
    if (arr->start + i >= arr->end)
        err_exit("index 'i' out of bounds");

    arr->contents[arr->start + i] = element;
}

void arr_clear(array *arr)
{
    if (arr->destructor) {
        for (size_t i = arr->start; i < arr->end; i++) {
            if (arr->contents[i])
                arr->destructor(arr->contents[i]);
        }
    }

    arr->start = 0;
    arr->end = 0;
}

void arr_free(void *a)
{
    array *arr = a;
    arr_clear(arr);
    free(arr->contents);
    free(arr);
}

static inline void arr_expand(array *arr)
{
    arr->cap *= 2;
    arr->contents = realloc(arr->contents, arr->cap * sizeof(void *));
    if (!arr->contents)
        err_exit("realloc error");

    // memset(arr->contents + arr->len, 0, arr->cap * sizeof(void *) / 2 );
}

void arr_push_(array *arr, void *element)
{
    if (arr->end == arr->cap)
        arr_expand(arr);

    arr->contents[arr->end++] = element;
}

void *arr_pop(array *arr)
{
    if (arr_len(arr) <= 0)
        err_exit("pop from empty array");

    return arr->contents[--arr->end];
}

void *arr_popleft(array *arr)
{
    if (arr_len(arr) <= 0)
        err_exit("popleft from empty array");

    return arr->contents[arr->start++];
}
