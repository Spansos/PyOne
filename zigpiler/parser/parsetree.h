#pragma once

#include <stddef.h>
#include "parser.h"

struct token {
    size_t line;
    size_t column;
    char * text;
    enum yytokentype type;
};
struct token * new_token(size_t line, size_t column, char * text, enum yytokentype type);
void free_token(struct token * token);
void print_token(struct token * token);

struct binary_operator {
    struct token * operator;
    struct expression * lhs;
    struct expression * rhs;
};
struct binary_operator * new_binary_operator(struct token * operator, struct expression * lhs, struct expression * rhs);
void free_binary_operator(struct binary_operator * binary_operator);
void print_binary_operator(struct binary_operator * binary_operator);

struct unary_operator {
    struct token * operator;
    struct expression * expression;
};
struct unary_operator * new_unary_operator(struct token * operator, struct expression * expression);
void free_unary_operator(struct unary_operator * unary_operator);
void print_unary_operator(struct unary_operator * unary_operator);

struct expression {
    union {
        struct token * value;
        struct binary_operator * binary_operator;
        struct unary_operator * unary_operator;
    };

    enum {
        VALUE,
        BINARY_OPERATOR,
        UNARY_OPERATOR
    } type;
};
struct expression * new_value_expression(struct token * token);
struct expression * new_binary_operator_expression(struct binary_operator * binary_operator);
struct expression * new_unary_operator_expression(struct unary_operator * unary_operator);
void free_expression(struct expression * expression);
void print_expression(struct expression * expression);