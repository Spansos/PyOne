#include <stdlib.h>
#include "token.h"


token_array_t create_token_array() {
    token_array_t array;

    array.capacity = 0;
    array.size = 0;
    array.tokens = calloc(1, sizeof(token_t));

    return array;
}


void push_token(token_array_t * array, token_t token) {
    array->size++;

    if (array->size > array->capacity) {
        array->capacity = array->capacity ? array->capacity * 2 : 1;
        array->tokens = realloc(array->tokens, (array->capacity+1) * sizeof(token_t));
    }

    array->tokens[array->size-1] = token;
    array->tokens[array->size] = (token_t){0};
}


void drop_comment_whitespace(token_array_t * array) {
    size_t index=0;

    for (token_t * i=array->tokens; i->type != INVALID; i++) {
        if (i->type == WHITESPACE) {
            array->size--;
        } else if (i->type == COMMENT) {
            array->size--;
        } else {
            array->tokens[index] = *i;
            index++;
        }
    }

    array->tokens[array->size] = (token_t){0};
}