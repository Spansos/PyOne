#pragma once

#include "ast.h"


typedef struct {
    token_t token;

    enum {
        EXPECTED_OPEN_BRACKET,
        EXPECTED_OPEN_CURLY_BRACKET,
        EXPECTED_CLOSING_SQUARE_BRACKET,
        EXPECTED_CLOSING_BRACKET,
        EXPECTED_EXPRESSION,
        EXPECTED_TYPE,
        EXPECTED_ASSIGNMENT,
        EXPECTED_IDENTIFIER,
        EXPECTED_WHILE,
        EXPECTED_FOR,
        EXPECTED_IF,
        EXPECTED_FUNCTION,
        EXPECTED_SEMICOLON,
        EXPECTED_ARROW
    } type;
} parse_error_t;


const char * error_type_string(parse_error_t error);
program_t * parse_program(token_array_t * tokens, parse_error_t * error);