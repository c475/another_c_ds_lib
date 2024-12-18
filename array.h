#ifndef __ARRAY_HEADER__
#define __ARRAY_HEADER__

#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "error.h"

#define ARRAY_DEFAULT_CAPACITY 64

typedef struct array {
    void **contents;
    size_t start;
    size_t end;
    size_t cap;
    void (*destructor)(void *);
} array;

array *arr_new(void (*destructor)(void *));
void arr_clear(array *arr);
void arr_free(void *a);
void arr_push_(array *arr, void *element);
void *arr_pop(array *arr);
void *arr_popleft(array *arr);
void *arr_get(array *arr, size_t i);
void *arr_getornull(array *arr, size_t i);
void *arr_take(array *arr, size_t i);
void arr_set(array *arr, void *element, size_t i);

#define arr_len(A) ((A->end) - (A->start))
#define arr_cap(A) ((A)->cap)
#define arr_push(A, E) arr_push_(A, (void *) E)
#define arr_empty(A) ((arr_len(A)) == 0)

// hacks that enable the foreach() macro
#define LINE_NAME( prefix ) JOIN( prefix, __LINE__ )
#define JOIN( symbol1, symbol2 ) _DO_JOIN( symbol1, symbol2 )
#define _DO_JOIN( symbol1, symbol2 ) symbol1##symbol2
#define foreach(A, T, E) size_t LINE_NAME(i) = A->start; \
    for ( \
        T E = (T) A->contents[ LINE_NAME(i) ]; \
        LINE_NAME(i) < (A->end); \
        LINE_NAME(i) ++, E = (T) A->contents[ LINE_NAME(i) ] \
    )

#endif
