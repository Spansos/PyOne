#include "generator.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lenstring.h"
#include "slice.h"
#include "lexer.h"

void generate_expression(expression_t * expression, struct lstr * out, const char * code);
void generate_statement(statement_t * statement, struct lstr * out, const char * code);

void generate_while(while_t * while_, struct lstr * out, const char * code) {
    // init
    ls_append_c(out, '(');

    // comprehension target
    ls_append_cstr(out, "{(");
    generate_statement(while_->statement, out, code);

    // stop condition
    ls_append_cstr(out, ",l()if b or r or not");
    generate_expression(while_->expression, out, code);
    ls_append_cstr(out, "else 0)[1]");

    // loop
    ls_append_cstr(out, "for l in L()},r,b:=0)[1]");
}

void generate_for(for_t * for_, struct lstr * out, const char * code) {
    // init
    ls_append_c(out, '(');
    generate_statement(for_->init, out, code);

    // comprehension target
    ls_append_cstr(out, ",{(");
    generate_statement(for_->body, out, code);
    ls_append_c(out, ',');
    generate_statement(for_->end, out, code);
    

    // stop condition
    ls_append_cstr(out, ",l()if b or r or not");
    generate_expression(for_->expression, out, code);
    ls_append_cstr(out, "else 0)[1]");

    // loop
    ls_append_cstr(out, "for l in L()},r,b:=0)[1]");
}

void generate_if(if_t * if_, struct lstr * out, const char * code) {
    switch (if_->type) {
        case IF_ONLY:
            ls_append_c(out, '(');
            generate_statement(if_->if_only.body, out, code);
            ls_append_cstr(out, "if");
            generate_expression(if_->if_only.condition, out, code);
            ls_append_cstr(out, "else 0)");
            break;
        
        case IF_ELSE:
            ls_append_c(out, '(');
            generate_statement(if_->if_else.body, out, code);
            ls_append_cstr(out, "if");
            generate_expression(if_->if_else.condition, out, code);
            ls_append_cstr(out, "else");
            generate_statement(if_->if_else.else_body, out, code);
            ls_append_cstr(out, ")");
            break;
        
        default:
            fprintf(stderr, "if wrong type: type %u\n", if_->type);
            exit(EXIT_FAILURE);
    }
}

void generate_function(function_t * function, struct lstr * out, const char * code) {
    ls_append_c(out, '(');

    // identifier
    ls_append_cstr(out, "___");
    slice_t slice;
    lex_single_token(code, function->identifier.start, &slice);
    ls_append_charp(out, slice.pointer, slice.length);
    
    ls_append_cstr(out, ":=lambda");

    // parameters
    for (struct Parameter * par=function->parameters; par->type!=NULL; par++) {
        ls_append_cstr(out, " ___");
        lex_single_token(code, par->identifier.start, &slice);
        ls_append_charp(out, slice.pointer, slice.length);
        ls_append_c(out, ',');
    }

    ls_append_c(out, ':');

    // local defs of r & b
    ls_append_c(out, '(');
    ls_append_cstr(out, "r:=R(),b:=0,");
    // statement
    generate_statement(function->body, out, code);
    ls_append_c(out, ')');

    // return value of body
    ls_append_cstr(out, "[2][0]");

    ls_append_c(out, ')');
}


void generate_block(block_t * block, struct lstr * out, const char * code) {
    ls_append_c(out, '(');

    for (statement_t ** i=block->statements; *i!=NULL; i++) {
        // or between statements
        if (i!=block->statements)
            ls_append_cstr(out, "or");
            
        generate_statement(*i, out, code);
    }

    ls_append_c(out, ')');
}

void generate_assignment(assignment_t * assignment, struct lstr * out, const char * code) {
    ls_append_c(out, '(');
    ls_append_cstr(out, "___");
    slice_t slice;
    lex_single_token(code, assignment->identifier.start, &slice);
    ls_append_charp(out, slice.pointer, slice.length);
    ls_append_cstr(out, ":=");
    generate_expression(assignment->expression, out, code);
    ls_append_c(out, ')');
}

void generate_declaration(declaration_t * declaration, struct lstr * out, const char * code) {
    generate_assignment(declaration->assignment, out, code);
}

void generate_primary(primary_t * primary, struct lstr * out, const char * code) {
    slice_t slice;
    switch (primary->type) {
        case INTEGER_:
            ls_append_c(out, '(');
            lex_single_token(code, primary->integer.start, &slice);
            ls_append_charp(out, slice.pointer, slice.length);
            ls_append_c(out, ')');
            break;
        case FLOATING_POINT_:
            ls_append_c(out, '(');
            lex_single_token(code, primary->floating_point.start, &slice);
            ls_append_charp(out, slice.pointer, slice.length);
            ls_append_c(out, ')');
            break;
        case STRING_:
            ls_append_c(out, '(');
            lex_single_token(code, primary->string.start, &slice);
            ls_append_charp(out, slice.pointer, slice.length);
            ls_append_c(out, ')');
            break;
        case IDENTIFIER_:
            ls_append_c(out, '(');
            lex_single_token(code, primary->string.start, &slice);
            ls_append_cstr(out, "___");
            ls_append_charp(out, slice.pointer, slice.length);
            ls_append_c(out, ')');
            break;
        case EXPRESSION:
            generate_expression(primary->expression, out, code);
            break;
        default:
            fprintf(stderr, "primary wrong type: type %u\n", primary->type);
            exit(EXIT_FAILURE);
    }
}

void generate_postfix(postfix_t * postfix, struct lstr * out, const char * code) {
    switch (postfix->type) {
        case FUNCTION_CALL:
            ls_append_c(out, '(');
            
            generate_primary(postfix->function_call.primary, out, code);
    
            ls_append_c(out, '(');
            for (expression_t ** i = postfix->function_call.arguments; *i != NULL; i++) {
                generate_expression(*i, out, code);
                ls_append_c(out, ',');
            }
            ls_append_c(out, ')');

            ls_append_c(out, ')');
            break;

        case INDEX:
            ls_append_c(out, '(');
            generate_primary(postfix->index.primary, out, code);
            ls_append_c(out, '[');
            generate_expression(postfix->index.index, out, code);
            ls_append_c(out, ']');
            ls_append_c(out, ')');
            break;

        case PRIMARY:
            generate_primary(postfix->primary, out, code);
            break;

        default:
            fprintf(stderr, "postfix wrong type: type %u\n", postfix->type);
            exit(EXIT_FAILURE);
    }
}

void generate_prefix(prefix_t * prefix, struct lstr * out, const char * code) {
    switch (prefix->type) {
        case PREFIX:
            ls_append_c(out, '(');
            switch (prefix->prefix.operator.type) {
                case MINUS:
                    ls_append_c(out, '-');
                    break;
                case PLUS:
                    ls_append_c(out, '+');
                    break;
                case NOT:
                    ls_append_cstr(out, "not ");
                    break;
                default:
                    fprintf(stderr, "prefix operator wrong type: type %u\n", prefix->prefix.operator.type);
                    exit(EXIT_FAILURE);
            }
            generate_prefix(prefix->prefix.prefix, out, code);
            ls_append_c(out, ')');
            break;

        case POSTFIX:
            generate_postfix(prefix->postfix, out, code);
            break;
        
        default:
            fprintf(stderr, "prefix wrong type: type %u\n", prefix->type);
            exit(EXIT_FAILURE);
    }
}


void generate_power(power_t * power, struct lstr * out, const char * code) {
    switch (power->type) {
        case POWER_:
            ls_append_c(out, '(');
            generate_prefix(power->power.prefix, out, code);
            if (power->power.operator.type != POWER) {
                fprintf(stderr, "power operator wrong type: type %u\n", power->power.operator.type);
                exit(EXIT_FAILURE);
            }
            ls_append_cstr(out, "**");
            generate_power(power->power.power, out, code);
            ls_append_c(out, ')');
            break;

        case PREFIX:
            generate_prefix(power->prefix, out, code);
            break;
        
        default:
            fprintf(stderr, "power wrong type: type %u\n", power->type);
            exit(EXIT_FAILURE);
    }
}

void generate_multiplication(multiplication_t * multiplication, struct lstr * out, const char * code) {
    switch (multiplication->type) {
        case MULTIPLICATION:
            ls_append_c(out, '(');
            generate_power(multiplication->multiplication.power, out, code);
            switch (multiplication->multiplication.operator.type) {
                case MULT:
                    ls_append_c(out, '*');
                    break;
                case DIVISION:
                    ls_append_c(out, '/');
                    break;
                case MODULUS:
                    ls_append_c(out, '%');
                    break;
                default:
                    fprintf(stderr, "multiplication operator wrong type: type %u\n", multiplication->multiplication.operator.type);
                    exit(EXIT_FAILURE);
            }
            generate_multiplication(multiplication->multiplication.multiplication, out, code);
            ls_append_c(out, ')');
            break;

        case POWER_:
            generate_power(multiplication->power, out, code);
            break;

        default:
            fprintf(stderr, "multiplication wrong type: type %u\n", multiplication->type);
            exit(EXIT_FAILURE);
    }
}

void generate_addition(addition_t * addition, struct lstr * out, const char * code) {
    switch (addition->type) {
        case ADDITION:
            ls_append_c(out, '(');
            generate_multiplication(addition->addition.multiplication, out, code);
            switch (addition->addition.operator.type) {
                case PLUS:
                    ls_append_c(out, '+');
                    break;
                case MINUS:
                    ls_append_c(out, '-');
                    break;
                default:
                    fprintf(stderr, "addition operator wrong type: type %u\n", addition->addition.operator.type);
                    exit(EXIT_FAILURE);
            }
            generate_addition(addition->addition.addition, out, code);
            ls_append_c(out, ')');
            break;

        case MULTIPLICATION:
            generate_multiplication(addition->multiplication, out, code);
            break;
        
        default:
            fprintf(stderr, "addition wrong type: type %u\n", addition->type);
            exit(EXIT_FAILURE);
    }
}


void generate_comparison(comparison_t * comparison, struct lstr * out, const char * code) {
    switch (comparison->type) {
        case COMPARISON:
            ls_append_c(out, '(');
            generate_addition(comparison->comparison.addition_lhs, out, code);
            switch (comparison->comparison.operator.type) {
                case EQUALS:
                    ls_append_cstr(out, "==");
                    break;
                case NOT_EQUALS:
                    ls_append_cstr(out, "!=");
                    break;
                case GREATER_EQUAL:
                    ls_append_cstr(out, ">=");
                    break;
                case LESSER_EQUAL:
                    ls_append_cstr(out, "<=");
                    break;
                case GREATER:
                    ls_append_c(out, '>');
                    break;
                case LESSER:
                    ls_append_c(out, '<');
                    break;
                default:
                    fprintf(stderr, "comparison operator wrong type: type %u\n", comparison->comparison.operator.type);
                    exit(EXIT_FAILURE);
            }
            generate_addition(comparison->comparison.addition_rhs, out, code);
            ls_append_c(out, ')');
            break;

        case ADDITION:
            generate_addition(comparison->addition, out, code);
            break;
            
        default:
            fprintf(stderr, "comparison wrong type: type %u\n", comparison->type);
            exit(EXIT_FAILURE);
    }
}


void generate_expression(expression_t * expression, struct lstr * out, const char * code) {
    switch (expression->type) {
        case LOGICAL:
            ls_append_c(out, '(');
            generate_comparison(expression->logical.comparison, out, code);
            switch (expression->logical.operator.type) {
                case AND:
                    ls_append_cstr(out, "and");
                    break;
                case OR:
                    ls_append_cstr(out, "or");
                    break;
                default:
                    fprintf(stderr, "logical operator wrong type: type %u\n", expression->logical.operator.type);
                    exit(EXIT_FAILURE);
            }
            generate_expression(expression->logical.expression, out, code);
            ls_append_c(out, ')');
            break;

        case COMPARISON:
            generate_comparison(expression->comparison, out, code);
            break;
        
        default:
            fprintf(stderr, "expression wrong type: type %u\n", expression->type);
            exit(EXIT_FAILURE);
    }
}


void generate_return(expression_t * return_, struct lstr * out, const char * code) {
    ls_append_cstr(out, "r:=(");
    generate_expression(return_, out, code);
    ls_append_cstr(out, ",)");
}


void generate_statement(statement_t * statement, struct lstr * out, const char * code) {
    ls_append_c(out, '(');
    
    // TODO:
    //  complex statements that don't block continues should should express to their expression
    switch (statement->type) {
        case WHILE_STATEMENT:
            generate_while(statement->while_, out, code);
            break;

        case FOR_STATEMENT:
            generate_for(statement->for_, out, code);
            break;

        case IF_STATEMENT:
            generate_if(statement->if_, out, code);
            break;

        case FUNCTION_STATEMENT:
            generate_function(statement->function, out, code);
            break;

        case BLOCK_STATEMENT:
            generate_block(statement->block, out, code);
            break;

        case ASSIGNMENT_STATEMENT:
            generate_assignment(statement->assignment, out, code);
            break;

        case declaration_STATEMENT:
            generate_declaration(statement->declaration, out, code);
            break;

        case EXPRESSION_STATEMENT:
            generate_expression(statement->expression, out, code);
            break;

        case RETURN_STATEMENT:
            generate_return(statement->return_statement, out, code);
            ls_append_c(out, ')');
            return;

        case BREAK_STATEMENT:
            ls_append_cstr(out, "b:=1)");
            return;

        case SKIP_STATEMENT:
            ls_append_cstr(out, "1)");
            return;

        default:
            fprintf(stderr, "statement wrong type: type %u\n", statement->type);
            exit(EXIT_FAILURE);
    }

    ls_append_cstr(out, ",r or b)[1]");
    
    return;
}


char * generate_code(program_t * program, const char * code) {
    struct lstr * out = ls_make_str();
    // stdlib should be a done
    
    for (statement_t ** i=program->statements; *i!=NULL; i++) {
        // or between statements
        if (i!=program->statements)
            ls_append_cstr(out, "or");
            
        generate_statement(*i, out, code);
    }

    char * r = strdup(ls_cstr(out));
    ls_free(&out);
    return r;
}