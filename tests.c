#include <string.h>
#include <assert.h>
#include "array.h"
#include "string.h"
#include "map.h"
#include "error.h"

/*

    ARRAY TESTS

*/
void test_arr_new(void)
{

}

void test_arr_clear(void)
{

}

void test_arr_free(void)
{

}

void test_arr_push_(void)
{

}

void test_arr_pop(void)
{

}

void test_arr_popleft(void)
{

}

void test_arr_get(void)
{

}


/*

    STRING TESTS

*/
void test_str_new(void)
{
    string *str = str_new();
    str_free(str);
}

void test_str_from(void)
{
    string *str = str_from("this is a test string");
    const char *other = "this is a test string";

    assert(strcmp(str_ptr(str), other) == 0);
    str_free(str);
}

void test_str_from_file(void)
{
    string *str = str_from_file("testfile.txt");
    const char *other = "this is some cool string";

    assert(strcmp(str_ptr(str), other) == 0);
    str_free(str);
}

void test_str_clone(void)
{
    string *str1 = str_from("this is a cloned string");
    string *str2 = str_clone(str1);

    assert(str_eq(str1, str2));
    assert(strcmp(str_ptr(str1), str_ptr(str2)) == 0);
    str_freeall(str1, str2);
}

void test_str_substr(void)
{
    string *str1 = str_from("make sure to substring me");
    string *str2 = str_substr(str1, 13, 22);

    assert(strcmp(str_ptr(str2), "substring") == 0);
    str_freeall(str1, str2);
}

void test_str_cat(void)
{
    string *str1 = str_from("hello ");
    string *str2 = str_from("world!");
    string *str3 = str_cat(str1, str2);

    assert(strcmp(str_ptr(str3), "hello world!") == 0);
    str_freeall(str1, str2, str3);
}

void test_str_split(void)
{
    const char *words[] = {"this", "", "is", "some", "text"};
    string *str = str_from("this##is#some#text");
    array *tokens = str_split(str, "#");
    for (size_t i = 0; i < arr_len(tokens); i++) {
        string *w = arr_get(tokens, i);
        assert(strcmp(str_ptr(w), words[i]) == 0);
    }

    str_free(str);
    arr_free(tokens);
}

void test_str_split_ws(void)
{
    const char *words[] = {"this", "is", "some", "text"};
    string *str = str_from("this   is\t\tsome\ntext");
    array *tokens = str_split_ws(str);
    for (size_t i = 0; i < arr_len(tokens); i++) {
        string *w = arr_get(tokens, i);
        assert(strcmp(str_ptr(w), words[i]) == 0);
    }

    str_free(str);
    arr_free(tokens);
}

void test_str_fmt(void)
{
    string *str1 = str_fmt("my name is %s and I am %d years old, here is a float: %g", "daniel", 33, 3.1415);
    string *str2 = str_from("my name is daniel and I am 33 years old, here is a float: 3.1415abcdefg");

    str_push(str1, 'a');
    str_push(str1, 'b');
    str_push(str1, 'c');
    str_push(str1, 'd');
    str_push(str1, 'e');
    str_push(str1, 'f');
    str_push(str1, 'g');

    str_print(str1);
    str_print(str2);

    assert(str_eq(str1, str2));
    str_freeall(str1, str2);
}

void test_str_push(void)
{
    string *str1 = str_new();
    const char *letters = "abcdefg";
    for (size_t i = 0; i < strlen(letters); i++) {
        str_push(str1, letters[i]);
    }

    assert(strcmp(str_ptr(str1), "abcdefg") == 0);
    str_free(str1);
}

void test_str_push_str(void)
{
    string *str1 = str_new();
    string *str2 = str_from("thesearesomewords");
    const char *words[] = {"these", "are", "some", "words"};
    for (size_t i = 0; i < 4; i++) {
        str_push_str(str1, words[i]);
    }

    assert(str_eq(str1, str2));
    str_freeall(str1, str2);
}

void test_str_pop(void)
{
    char prev, tmp;
    string *str = str_from("abcdefg");

    prev = str_pop(str);
    while (str_len(str) > 0) {
        tmp = str_pop(str);
        assert(tmp + 1 == prev);
        prev = tmp;
    }

    str_free(str);
}

void test_str_popleft(void)
{
    char prev, tmp;
    string *str = str_from("abcdefg");

    prev = str_popleft(str);
    while (str_len(str) > 0) {
        tmp = str_popleft(str);
        assert(tmp == prev + 1);
        prev = tmp;
    }

    str_free(str);
}

void test_str_bounds(void)
{
    string *str = str_from("make sure to substring me");

    str_bounds(str, 13, 22);

    assert(strcmp(str_ptr(str), "substring") == 0);
    str_free(str);
}

void test_str_trim(void)
{
    string *str1 = str_from("   some text is here   \t\n");
    string *str2 = str_from("some text is here");

    str_trim(str1);

    assert(str_eq(str1, str2));
    str_freeall(str1, str2);
}

void test_str_trunc(void)
{
    string *str = str_from("this is some test string");

    str_trunc(str, 7);

    assert(strcmp(str_ptr(str), "this is") == 0);
    str_free(str);
}

void test_str_clear(void)
{
    string *str = str_from("this is another test");

    str_clear(str);

    assert(str_len(str) == 0);
    assert(strcmp(str_ptr(str), "") == 0);
    str_free(str);
}

void test_str_free(void)
{
    string *str = str_from("this is another test");

    str_free(str);
}

void test_str_tolower(void)
{
    string *str = str_from("THIS IS ALL CAPS");
    str_tolower(str);

    assert(strcmp(str_ptr(str), "this is all caps") == 0);
    str_free(str);
}

void test_str_toupper(void)
{
    string *str = str_from("this is lowercase");
    str_toupper(str);

    assert(strcmp(str_ptr(str), "THIS IS LOWERCASE") == 0);
    str_free(str);
}

void test_str_eq(void)
{
    string *str1 = str_from("this is a test string");
    string *str2 = str_from("this is a test string");

    assert(str_eq(str1, str2));
    str_freeall(str1, str2);
}

void test_str_get(void)
{
    char prev, tmp;
    string *str = str_from("abcdefg");
    prev = str_get(str, 0);
    for (size_t i = 1; i < str_len(str); i++) {
        tmp = str_get(str, i);
        assert(tmp == prev + 1);
        prev = tmp;
    }

    str_free(str);
}


/*

    MAP TESTS

*/
void test_map_new(void)
{
    map *m = map_new(NULL, NULL, NULL, NULL);
    map_free(m);
}

int str_cmp(void *a, void *b)
{
    string *sa = a;
    string *sb = b;
    return (int)str_eq(sa, sb);
}
void test_map_insert(void)
{
    map *m;
    array *words;
    string *contents;
    bool *contains;
    size_t i, j;

    contents = str_from_file("dictionary.txt");
    words = str_split_ws(contents);

    m = map_new(str_cmp, str_hash, str_free, NULL);
    for (i = 0; i < arr_len(words); i++) {
        string *s = arr_take(words, i);
        map_insert(m, s, (void *)i);
    }

    contains = calloc(i, sizeof(bool));
    if (!contains)
        err_exit("calloc");

    foreach_map(m, node) {
        size_t value = (size_t)node->value;
        contains[value] = true;
    }

    for (j = 0; j < i; j++)
        if (!contains[j])
            err_exit("contains[%zu] is false", j);

    free(contains);
    map_free(m);
    str_free(contents);
    arr_free(words);
}

int other_strcmp(void *a, void *b)
{
    const char *sa = a;
    const char *sb = b;
    return strcmp(sa, sb);
}
void test_map_get(void)
{
    size_t i;
    char *keys[] = {"these", "are", "some", "keys"};
    size_t values[] = {1, 2, 3, 4};
    map *m = map_new(other_strcmp, NULL, NULL, NULL);

    for (i = 0; i < 4; i++) {
        map_insert(m, keys[i], (void *)values[i]);
    }

    assert((size_t)map_get(m, "these") == 1);
    assert((size_t)map_get(m, "are") == 2);
    assert((size_t)map_get(m, "some") == 3);
    assert((size_t)map_get(m, "keys") == 4);

    map_free(m);
}

void test_map_delete(void)
{
    size_t i;
    char *keys[] = {"these", "are", "some", "keys"};
    size_t values[] = {1, 2, 3, 4};
    map *m = map_new(other_strcmp, NULL, NULL, NULL);

    for (i = 0; i < 4; i++) {
        map_insert(m, keys[i], (void *)values[i]);
    }

    map_delete(m, "these");

    assert(map_get(m, "these") == NULL);

    map_free(m);
}

void test_map_contains(void)
{
    size_t i;
    char *keys[] = {"these", "are", "some", "keys"};
    size_t values[] = {1, 2, 3, 4};
    map *m = map_new(other_strcmp, NULL, NULL, NULL);

    for (i = 0; i < 4; i++) {
        map_insert(m, keys[i], (void *)values[i]);
    }

    assert(map_contains(m, "these"));
    assert(!map_contains(m, "does not exist"));

    map_free(m);
}

int main(int argc, char *argv[])
{
    /* array tests */

    /* string tests */
    test_str_new();
    test_str_from();
    test_str_from_file();
    test_str_clone();
    test_str_substr();
    test_str_cat();
    test_str_split();
    test_str_split_ws();
    test_str_fmt();
    test_str_push();
    test_str_push_str();
    test_str_pop();
    test_str_popleft();
    test_str_bounds();
    test_str_trim();
    test_str_trunc();
    test_str_clear();
    test_str_free();
    test_str_tolower();
    test_str_toupper();
    test_str_eq();
    test_str_get();

    /* map tests */
    test_map_new();
    test_map_contains();
    test_map_delete();
    test_map_get();
    test_map_insert();

    printf("tests passed\n");

    return 0;
}
