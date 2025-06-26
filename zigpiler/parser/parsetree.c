#include "parsetree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int indent = 0;
unsigned int tab_size = 2;

void print_indent() {
    for (int i=0; i<indent*tab_size; i++)
        printf(" ");
    // char s = '\0';
    // printf("%*s", indent*tab_size, &s);
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
    printf("--- Binary Operator ---\n");
    
    indent++;
    
    print_indent();
    printf("- Operator -\n");
    indent++;
    print_token(binary_operator->operator);
    indent--;
    
    print_indent();
    printf("- Left-hand side -\n");
    indent++;
    print_expression(binary_operator->lhs);
    indent--;
    
    print_indent();
    printf("- Right-hand side -\n");
    indent++;
    print_expression(binary_operator->rhs);
    indent--;
    
    indent--;
}

struct unary_operator * new_unary_operator(struct token * operator, struct expression * expression) {
    struct unary_operator * unary_operator = malloc(sizeof(struct unary_operator));

    unary_operator->operator = operator;
    unary_operator->expression = expression;

    return unary_operator;
}

void free_unary_operator(struct unary_operator * unary_operator) {
    free_expression(unary_operator->expression);
    free_token(unary_operator->operator);
    free(unary_operator);
}

void print_unary_operator(struct unary_operator * unary_operator) {
    print_indent();
    printf("--- Unary Operator ---\n");
    
    indent++;
    
    print_indent();
    printf("- Operator -\n");
    indent++;
    print_token(unary_operator->operator);
    indent--;

    print_indent();
    printf("- Expression -\n");
    print_expression(unary_operator->expression);
    
    indent--;
}


struct expression * new_value_expression(struct token * token) {
    struct expression * expression = malloc(sizeof(struct expression));

    expression->type = VALUE;
    expression->value = token;

    return expression;
}

struct expression * new_binary_operator_expression(struct binary_operator * binary_operator) {
    struct expression * expression = malloc(sizeof(struct expression));

    expression->type = BINARY_OPERATOR;
    expression->binary_operator = binary_operator;

    return expression;
}

struct expression * new_unary_operator_expression(struct unary_operator * unary_operator) {
    struct expression * expression = malloc(sizeof(struct expression));

    expression->type = UNARY_OPERATOR;
    expression->unary_operator = unary_operator;

    return expression;
}

void free_expression(struct expression * expression) {
    switch (expression->type) {
        case VALUE:
            free_token(expression->value);
            break;
        case BINARY_OPERATOR:
            free_binary_operator(expression->binary_operator);
            break;
        case UNARY_OPERATOR:
            free_unary_operator(expression->unary_operator);
            break;
    }
    free(expression);
}

void print_expression(struct expression * expression) {
    print_indent();
    printf("--- expression ---\n");
    
    indent++;
    
    switch (expression->type) {
        case VALUE:
            print_token(expression->value);
            break;
        case BINARY_OPERATOR:
            print_binary_operator(expression->binary_operator);
            break;
        case UNARY_OPERATOR:
            print_unary_operator(expression->unary_operator);
            break;
    }

    indent--;
}