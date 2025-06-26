#include "parsetree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int indent = 0;
unsigned int tab_size = 2;

void print_indent() {
    printf("%*s", indent*tab_size, NULL);
}

struct token * new_token(size_t line, size_t column, char * text, enum yytokentype type) {
    struct token * token = malloc(sizeof(struct token));

    token->line = line;
    token->column = column;
    token->text = strdup(text);
    token->type = type;

    return token;
}

void free_token(struct token * token) {
    free(token->text);
    free(token);
}

void print_token(struct token * token) {
    print_indent();
    printf("%zu:%zu <%s> %s\n");
}

struct binary_operator * new_binary_operator(struct token * operator, struct expression * lhs, struct expression * rhs) {
    struct binary_operator * binary_operator = malloc(sizeof(struct binary_operator));

    binary_operator->operator = operator;
    binary_operator->lhs = lhs;
    binary_operator->rhs = rhs;

    return binary_operator;
}

void free_binary_operator(struct binary_operator * binary_operator) {
    free_expression(binary_operator->lhs);
    free_expression(binary_operator->rhs);
    free_token(binary_operator->operator);
    free(binary_operator);
}

void print_binary_operator(struct binary_operator * binary_operator) {
    print_indent();
    printf("Binary Operator");
    
    indent++;
    
    print_indent();
    printf("Operator");
    print_token(binary_operator->operator);

    print_indent();
    printf("Left-hand side");
    print_expression(binary_operator->lhs);
    
    print_indent();
    printf("Right-hand side");
    print_expression(binary_operator->rhs);
    
    indent--;
}

struct unary_operator * new_unary_operator(struct token * operator, struct expression * expression) {
    struct unary_operator * unary_operator = malloc(sizeof(struct unary_operator));

    unary_operator->operator = operator;
    unary_operator->expression = expression;

    return unary_operator;
}

void free_unary_operator(struct unary_operator * unary_operator);
void print_unary_operator(struct unary_operator * unary_operator);


struct expression * new_value_expression(struct token * token);
struct expression * new_binary_operator_expression(struct binary_operator * binary_operator);
struct expression * new_unary_operator_expression(struct unary_operator * unary_operator);
void free_expression_operator(struct expression * expression);
void print_expression_operator(struct expression * expression);