#pragma once

#include <stdbool.h>
#include "token.h"

typedef enum {
    EXPRESSION,
    INTEGER_,
    FLOATING_POINT_,
    STRING_,
    IDENTIFIER_,
    PRIMARY,
    FUNCTION_CALL,
    INDEX,
    PREFIX,
    POSTFIX,
    POWER_,
    MULTIPLICATION,
    ADDITION,
    COMPARISON,
    LOGICAL,
    REFERENCE,
    DATA_TYPE,
    IF_ONLY,
    IF_ELSE,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    IF_STATEMENT,
    FUNCTION_STATEMENT,
    BLOCK_STATEMENT,
    ASSIGNMENT_STATEMENT,
    declaration_STATEMENT,
    EXPRESSION_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    SKIP_STATEMENT
} nodetype_t;

typedef struct Primary primary_t;
typedef struct Postfix postfix_t;
typedef struct Prefix prefix_t;
typedef struct Power power_t;
typedef struct Multiplication multiplication_t;
typedef struct Addition addition_t;
typedef struct Comparison comparison_t;
typedef struct Expression expression_t;
typedef struct Type type_t;
typedef struct Assignment assignment_t;
typedef struct Declaration declaration_t;
typedef struct While while_t;
typedef struct For for_t;
typedef struct If if_t;
typedef struct Function function_t;
typedef struct Block block_t;
typedef struct Statement statement_t;
typedef struct Program program_t;


typedef struct Primary {
    union {
        expression_t * expression;
        token_t integer;
        token_t floating_point;
        token_t string;
        token_t identifier;
    };

    nodetype_t type;

} primary_t;


typedef struct Postfix {
    union {
        primary_t * primary;

        struct {
            primary_t * primary;
            expression_t ** arguments;
        } function_call;

        struct {
            primary_t * primary;
            expression_t * index;
        } index;
    };

    nodetype_t type;
    
} postfix_t;


typedef struct Prefix {
    union {
        struct {
            token_t operator;
            prefix_t * prefix;
        } prefix;

        postfix_t * postfix;
    };

    nodetype_t type;
    
} prefix_t;


typedef struct Power {
    union {
        prefix_t * prefix;
        
        struct {
            token_t operator;
            prefix_t * prefix;
            power_t * power;
        } power;
    };

    nodetype_t type;

} power_t;


typedef struct Multiplication {
    union {
        power_t * power;
        
        struct {
            power_t * power;
            token_t operator;
            multiplication_t * multiplication;
        } multiplication;

    };

    nodetype_t type;

} multiplication_t;


typedef struct Addition {
    union {
        multiplication_t * multiplication;

        struct {
            multiplication_t * multiplication;
            token_t operator;
            addition_t * addition;
        } addition;
    };

    nodetype_t type;
     
} addition_t;


typedef struct Comparison {
    union {
        addition_t * addition;

        struct {
            addition_t * addition_lhs;
            token_t operator;
            addition_t * addition_rhs;
        } comparison;
    };

    nodetype_t type;

} comparison_t;


typedef struct Expression {
    union {
        comparison_t * comparison;

        struct {
            comparison_t * comparison;
            token_t operator;
            expression_t * expression;
        } logical;
    };

    nodetype_t type;

} expression_t;


typedef struct Type {
    union {
        struct {
            bool mut;
            token_t identifier;
        } data_type;

        struct {
            bool mut;
            type_t * type;
        } reference;
    };

    nodetype_t type;

} type_t;


typedef struct Assignment {
    token_t identifier;
    expression_t * expression;
} assignment_t;


typedef struct Declaration {
    type_t * type;
    assignment_t * assignment;
} declaration_t;


typedef struct While {
    expression_t * expression;
    statement_t * statement;
} while_t;


typedef struct For {
    statement_t * init;
    expression_t * expression;
    statement_t * end;
    statement_t * body;
} for_t;


typedef struct If {
    union {
        struct {
            expression_t * condition;
            statement_t * body;
        } if_only;

        struct {
            expression_t * condition;
            statement_t * body;
            statement_t * else_body;
        } if_else;
    };

    nodetype_t type;

} if_t;


typedef struct Function {
    token_t identifier;
    type_t * return_type;
    statement_t * body;
    struct Parameter {
        type_t * type;
        token_t identifier;
    } * parameters;
} function_t;


typedef struct Block {
    statement_t ** statements;
} block_t;


typedef struct Statement {
    union {
        while_t * while_;
        for_t * for_;
        if_t * if_;
        function_t * function;
        block_t * block;
        assignment_t * assignment;
        declaration_t * declaration;
        expression_t * expression;
        expression_t * return_statement;
    };

    nodetype_t type;

} statement_t;


typedef struct Program {
    statement_t ** statements;
} program_t;


void free_primary(primary_t * primary);
void free_postfix(postfix_t * postfix);
void free_prefix(prefix_t * prefix);
void free_power(power_t * power);
void free_multiplication(multiplication_t * multiplication);
void free_addition(addition_t * addition);
void free_comparison(comparison_t * comparison);
void free_expression(expression_t * expression);
void free_type(type_t * type);
void free_assignment(assignment_t * assignment);
void free_declaration(declaration_t * declaration);
void free_while(while_t * while_);
void free_for(for_t * for_);
void free_if(if_t * if_);
void free_function(function_t * function);
void free_block(block_t * block);
void free_statement(statement_t * statement);
void free_program(program_t * program);

void print_primary(primary_t * primary);
void print_postfix(postfix_t * postfix);
void print_prefix(prefix_t * prefix);
void print_power(power_t * power);
void print_multiplication(multiplication_t * multiplication);
void print_addition(addition_t * addition);
void print_comparison(comparison_t * comparison);
void print_expression(expression_t * expression);
void print_type(type_t * type);
void print_assignment(assignment_t * assignment);
void print_declaration(declaration_t * declaration);
void print_while(while_t * while_);
void print_for(for_t * for_);
void print_if(if_t * if_);
void print_function(function_t * function);
void print_block(block_t * block);
void print_statement(statement_t * statement);
void print_program(program_t * program);