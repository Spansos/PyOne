#include "lenstring.h"

#include <stdlib.h>
#include <string.h>


struct lstr {
    size_t length;
    size_t capacity;
    char * str;
};




size_t ls_length(const struct lstr * str) {
    return str->length;
}

char * ls_charp(const struct lstr * str) {
    return str->str;
}

char * ls_cstr(const struct lstr * str) {
    return str->str;
}




struct lstr * ls_copy(const struct lstr * str) {
    struct lstr * new = malloc(sizeof(struct lstr));

    new->length   = str->length;
    new->capacity = str->capacity;
    new->str      = malloc(str->capacity);

    memcpy(new->str, str->str, str->length+1);

    return new;
}

struct lstr * ls_make_str() {
    struct lstr * new = malloc(sizeof(struct lstr));

    new->length   = 0;
    new->capacity = 1;
    new->str      = malloc(2);

    new->str[0] = '\0';

    return new;
}

struct lstr * ls_make_str_cstr(const char * cstr) {
    struct lstr * new = malloc(sizeof(struct lstr));

    new->length   = strlen(cstr);
    new->capacity = new->length;
    new->str      = malloc(new->capacity+1);

    strcpy(new->str, cstr);

    return new;
}

struct lstr * ls_make_str_charp(const char * str, size_t len) {
    struct lstr * new = malloc(sizeof(struct lstr));

    new->length   = len;
    new->capacity = len;
    new->str      = malloc(len+1);

    memcpy(new->str, str, len);
    new->str[len] = '\0';

    return new;
}




// free a lstr
void ls_free(struct lstr ** lstr) {
    free((*lstr)->str);
    free(*lstr);
    *lstr = NULL;
}




bool ls_append_c(struct lstr * str, char c) {
    if (str->length+1 > str->capacity) {
        str->capacity *= 2;
        str->str = realloc(str->str, str->capacity+1);
    }

    str->str[str->length++] = c;
    str->str[str->length] = '\0';

    return false;
}

bool ls_append_cstr(struct lstr * str, const char * cstr) {
    // im lazy. could be so much more efffficient
    for (; *cstr; cstr++)
        ls_append_c(str, *cstr);
    
    return false;
}

bool ls_append_lstr(struct lstr * str, const struct lstr * str2) {
    // im lazy. could be so much more efffficient
    char * cstr = ls_cstr(str2);
    return ls_append_cstr(str, cstr);
}

bool ls_append_charp(struct lstr * str, const char * charp, size_t len) {
    // im lazy. could be so so much more efffficient
    struct lstr * a = ls_make_str_charp(charp, len);
    bool r = ls_append_lstr(str, a);
    ls_free(&a);
    return r;
}