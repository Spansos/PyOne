#pragma once

#include <stddef.h>

typedef enum TokenType {
    INVALID = 0,
    COMMENT,
    WHITESPACE,
    AND,
    OR, 
    EQUALS,
    NOT_EQUALS,
    GREATER_EQUAL,
    LESSER_EQUAL,
    GREATER,
    LESSER, 
    PLUS,
    MINUS, 
    MULT,
    DIVISION,
    MODULUS, 
    POWER,
    NOT, 
    BRACKET_OPEN,
    COMMA,
    BRACKET_CLOSE,
    SQR_BRACKET_OPEN,
    SQR_BRACKET_CLOSE, 
    INT,
    FLOAT,
    STRING,
    IDENTIFIER,
    MUT,
    REF,
    ASSIGN, 
    WHILE,
    FOR,
    SEMICOLON,
    ARROW,
    IF,
    ELSE, 
    FUNCTION,
    CURLY_BRACKET_OPEN,
    CURLY_BRACKET_CLOSE,
    RETURN,
    BREAK,
    SKIP,
} token_type_t;


typedef struct {
    size_t start;
    token_type_t type;
} token_t;


typedef struct {
    size_t capacity;
    size_t size;
    token_t * tokens;
} token_array_t;

token_array_t create_token_array();
void push_token(token_array_t * array, token_t token);
void drop_comment_whitespace(token_array_t * array);