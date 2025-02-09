#pragma once

#include <stddef.h>
#include <stdbool.h>

struct lstr;

// get length of str
size_t ls_length(const struct lstr * str);
// get chars of string as char pointer
// may or may not be zero-terminated
char * ls_charp(const struct lstr * str);
// get chars of string as c string
// same as ls_charp but guaranteed to be zero-terminated
char * ls_cstr(const struct lstr * str);

// make a copy of str
struct lstr * ls_copy(const struct lstr * str);
// make str
struct lstr * ls_make_str();
// make str from c string
struct lstr * ls_make_str_cstr(const char * cstr);
// make str from char pointer with length
struct lstr * ls_make_str_charp(const char * str, size_t len);

// frees the string at *str. sets *str to NULL, to help find potential errors;
void ls_free(struct lstr ** str);

// append char to str in place. returns failure
bool ls_append_c(struct lstr * str, char c);
// append c string to str in place. returns failure
bool ls_append_cstr(struct lstr * str, const char * cstr);
// append str2 to str in palce. returns failure
bool ls_append_lstr(struct lstr * str, const struct lstr * str2);
// append char pointer with length to str in place. returns failure
bool ls_append_charp(struct lstr * str, const char * charp, size_t len);