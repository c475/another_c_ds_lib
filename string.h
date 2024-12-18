#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include "error.h"
#include "array.h"

#define STRING_DEFAULT_CAP 32
#define STR_EXPAND_FACTOR 2

typedef struct string {
    char *contents;
    size_t start;
    size_t end;
    size_t cap;
} string;

#define str_len(S) (S->end - S->start)
#define str_ptr(S) (S->contents + S->start)
#define whitespace(C) ((C == ' ') || (C == '\t') || (C == '\n') || (C == '\r'))

string *str_new(void);
string *str_from(const char *from);
string *str_from_file(const char *pathname);
string *str_clone(string *str);
string *str_substr(string *str, size_t start, size_t end);
string *str_cat(string *s1, string *s2);
array *str_split(string *str, const char *delim);
array *str_split_ws(string *str);
string *str_fmt(const char *fmt, ...);
void str_push(string *str, char c);
void str_push_str(string *str, const char *other);
char str_pop(string *str);
char str_popleft(string *str);
void str_bounds(string *str, int start, int end);
void str_trim(string *str);
void str_trunc(string *str, size_t len);
void str_clear(string *str);
void str_free(void *s);
void str_freeall_(void *s, ...);
void str_tolower(string *str);
void str_toupper(string *str);
bool str_eq(string *s1, string *s2);
uint64_t str_hash(void *s);
char str_get(string *str, size_t i);
void str_print(string *str);

#define str_freeall(S, ...) str_freeall_((void *) S, ##__VA_ARGS__, NULL)

#define foreach_str(S, C) size_t LINE_NAME(i) = S->start; \
    for ( \
        char C = S->contents[ LINE_NAME(i) ]; \
        C && LINE_NAME(i) < S->end; \
        LINE_NAME(i) ++, C = S->contents[ LINE_NAME(i) ] \
    )

#define foreach_strp(S, C) size_t LINE_NAME(i) = S->start; \
    for ( \
        char *C = &S->contents[ LINE_NAME(i) ]; \
        *C && LINE_NAME(i) < S->end; \
        LINE_NAME(i) ++, C = &S->contents[ LINE_NAME(i) ] \
    )
