#include "string.h"

string *str_new(void)
{
    string *str = calloc(1, sizeof(string));
    if (!str)
        err_exit("calloc error (struct)");

    str->contents = calloc(1, STRING_DEFAULT_CAP);
    if (!str->contents)
        err_exit("calloc error (contents)");

    str->cap = STRING_DEFAULT_CAP;
    return str;
}

string *str_from(const char *from)
{
    string *str;
    size_t len;
    size_t cap;

    str = calloc(1, sizeof(string));
    if (!str)
        err_exit("calloc error (struct)");

    len = strlen(from);
    cap = STRING_DEFAULT_CAP * (len / STRING_DEFAULT_CAP + 1);

    str->contents = calloc(1, cap);
    if (!str->contents)
        err_exit("calloc error (contents)");

    str->end = len;
    str->cap = cap;

    strncpy(str->contents, from, len);

    return str;
}

string *str_from_file(const char *pathname)
{
    int fd;
    char *contents;
    struct stat st;
    ssize_t num_read;
    size_t total, cap;
    string *str;

    if (stat(pathname, &st) == -1)
        err_exit("stat() failed for %s", pathname);

    fd = open(pathname, O_RDWR);
    if (fd == -1)
        err_exit("Couldn't open %s for reading", pathname);

    cap = st.st_size + 512;
    contents = calloc(1, cap);
    if (!contents)
        err_exit("calloc failed");

    num_read = 0;
    total = 0;
    while (total < st.st_size) {
        num_read = read(fd, contents + total, st.st_size);
        if (num_read == -1)
            err_exit("read failed");
        total += num_read;
    }

    if (close(fd) == -1)
        err_exit("CLOSE ACTUALLY FAILED!");

    str = calloc(1, sizeof(string));
    if (!str)
        err_exit("calloc");

    str->contents = contents;
    str->end = st.st_size;
    str->cap = cap;

    return str;
}

string *str_read(int fd, size_t size)
{
    string *str;
    char *contents;
    ssize_t num_read;

    str = calloc(1, sizeof(string));
    if (!str)
        err_exit("calloc");

    str->cap = size;

    contents = calloc(1, size + 1);
    if (!contents)
        err_exit("calloc");

    num_read = read(fd, contents, size);
    if (num_read == -1)
        err_exit("read");

    str->end = num_read;

    return str;
}

string *str_clone(string *str)
{
    string *new_str;

    new_str = calloc(1, sizeof(string));
    if (!new_str)
        err_exit("calloc error (struct)");

    new_str->contents = calloc(1, str->cap);
    if (!new_str->contents)
        err_exit("calloc (contents)");

    strncpy(new_str->contents, str->contents + str->start, str_len(str));

    new_str->end = str_len(str);
    new_str->cap = str->cap;

    return new_str;
}

string *str_substr(string *str, size_t start, size_t end)
{
    string *new_str;
    char tmp;

    tmp = str->contents[str->start + end];
    str->contents[str->start + end] = '\0';
    new_str = str_from(str_ptr(str) + start);
    str->contents[str->start + end] = tmp;

    return new_str;
}

string *str_cat(string *s1, string *s2)
{
    string *str;
    size_t len1, len2;

    str = calloc(1, sizeof(string));
    if (!str)
        err_exit("calloc error (struct)");

    str->cap = s1->cap + s2->cap;
    str->contents = calloc(1, str->cap);
    if (!str->contents)
        err_exit("calloc error (contents)");

    len1 = str_len(s1);
    len2 = str_len(s2);

    strncpy(str->contents, s1->contents + s1->start, len1);
    strncpy(str->contents + len1, s2->contents + s2->start, len2);
    str->end = len1 + len2;

    return str;
}

// look for bugs here
string *str_fmt(const char *fmt, ...)
{
    va_list va1, va2;
    string *str;
    char *contents;
    size_t n;

    va_start(va1, fmt);
    va_copy(va2, va1);

    n = vsnprintf(NULL, 0, fmt, va1);
    n = n > STRING_DEFAULT_CAP ? n + 1 : STRING_DEFAULT_CAP;
    va_end(va1);

    contents = calloc(1, n * 2);
    if (!contents)
        err_exit("calloc 1");

    vsnprintf(contents, n + 1, fmt, va2);
    va_end(va2);

    str = calloc(1, sizeof(string));
    if (!str)
        err_exit("calloc 2");

    str->cap = n * 2;
    str->end = n - 1;
    str->contents = contents;

    return str;
}

static void str_expand(string *str)
{
    str->cap *= STR_EXPAND_FACTOR;
    str->contents = realloc(str->contents, str->cap);
    if (!str->contents)
        err_exit("realloc error");
}

void str_push(string *str, char c)
{
    if (str->end == str->cap)
        str_expand(str);

    str->contents[str->end++] = c;
    str->contents[str->end] = '\0';
}

void str_push_str(string *str, const char *other)
{
    size_t len = strlen(other);

    // kind of lame but this will be fine usually
    // grow if the remaining space isn't enough for len + 1 for the null byte
    while (str->cap - str->end <= len + 1)
        str_expand(str);

    strncpy(str->contents + str->end, other, len);
    str->end += len;
    str->contents[str->end] = '\0';
}

char str_pop(string *str)
{
    char c;

    if (!str_len(str))
        err_exit("pop from empty string");

    c = str->contents[--str->end];
    str->contents[str->end] = '\0';
    return c;
}

char str_popleft(string *str)
{
    if (!str_len(str))
        err_exit("pop from empty string");

    return str->contents[str->start++];
}

// like str_substr() but doesn't allocate a new string
void str_bounds(string *str, int start, int end)
{
    if (start > end)
        err_exit("start > end");

    str->start = start;
    str->end = end;
    str->contents[str->end] = '\0';
}

void str_trim(string *str)
{
    while (str_len(str) && whitespace(str->contents[str->start]))
        str->start++;

    while (str_len(str) && whitespace(str->contents[str->end-1]))
        str->end--;

    str->contents[str->end] = '\0';
}

void str_trunc(string *str, size_t len)
{
    if (str_len(str) <= len)
        return;

    str->contents[str->start + len] = '\0';
    str->end = str->start + len;
}

void str_clear(string *str)
{
    str->end = str->start;
    str->contents[str->end] = '\0';
}

void str_free(void *s)
{
    string *str = s;
    free(str->contents);
    free(str);
}

void str_freeall_(void *s, ...)
{
    va_list va;
    void *str;

    va_start(va, s);
    str = va_arg(va, void *);
    while (str != NULL) {
        str_free(str);
        str = va_arg(va, void *);
    }

    va_end(va);
    str_free(s);
}

void str_tolower(string *str)
{
    foreach_strp(str, c) if (*c >= 'A' && *c <= 'Z') *c += ('a' - 'A');
}

void str_toupper(string *str)
{
    foreach_strp(str, c) if (*c >= 'a' && *c <= 'z') *c -= ('a' - 'A');
}

array *str_split(string *str, const char *delim)
{
    size_t len;
    char *base, *match;
    size_t start_i;
    array *results;
    string *new_str;

    len = strlen(delim);
    if (!len)
        err_exit("empty deliminator");

    results = arr_new(str_free);
    if (!results)
        err_exit("arr() failure");

    base = str_ptr(str);
    start_i = 0;
    match = strstr(base, delim);
    while (match) {
        new_str = str_substr(str, start_i, start_i + match - base);
        arr_push(results, new_str);
        start_i += match - base + len;
        base = match + len;
        match = strstr(base, delim);
    }

    new_str = str_substr(str, start_i, str->end);
    arr_push(results, new_str);

    return results;
}

array *str_split_ws(string *str)
{
    char tmp;
    string *new_str;
    size_t start, i, old_start, old_end;
    array *results;

    old_start = str->start;
    old_end = str->end;
    str_trim(str);

    results = arr_new(str_free);
    start = i = str->start;
    while (i < str->end) {
        if (whitespace(str->contents[i])) {
            tmp = str->contents[i];
            str->contents[i] = '\0';
            new_str = str_from(str->contents + start);
            str->contents[i] = tmp;
            arr_push(results, new_str);

            while (i < str->end - 1 && whitespace(str->contents[i])) i++;
            start = i;
        }

        i++;
    }

    tmp = str->contents[i];
    str->contents[i] = '\0';
    new_str = str_from(str->contents + start);
    str->contents[i] = tmp;
    arr_push(results, new_str);

    str->start = old_start;
    str->end = old_end;

    return results;
}

bool str_eq(string *s1, string *s2)
{
    size_t len1, len2;

    len1 = strlen(str_ptr(s1));
    len2 = strlen(str_ptr(s2));
    if (len1 != len2)
        return false;

    return strncmp(str_ptr(s1), str_ptr(s2), len1) == 0;
}

uint64_t str_hash(void *s)
{
    string *str = s;
    uint64_t hash = 0xcbf29ce484222325;
    for (size_t i = str->start; i < str->end; i++) {
        hash ^= str->contents[i];
        hash *= 0x100000001b3;
    }

    return hash;
}

char str_get(string *str, size_t i)
{
    if (i + str->start >= str->end)
        err_exit("str_get() OOB");

    return str->contents[str->start + i];
}

void str_print(string *str)
{
    printf("%s\n", str_ptr(str));
}
