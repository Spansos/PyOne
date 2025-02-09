#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

void free_primary(primary_t * primary) {
    if (primary->type == EXPRESSION)
        free_expression(primary->expression);
    
    free(primary);
}


void free_postfix(postfix_t * postfix) {
    switch (postfix->type) {
        case PRIMARY:
            free_primary(postfix->primary);
            break;

        case FUNCTION_CALL:
            free_primary(postfix->function_call.primary);
            for (expression_t ** i = postfix->function_call.arguments; *i != NULL; i++)
                free_expression(*i);
            free(postfix->function_call.arguments);
            break;

        case INDEX:
            free_primary(postfix->index.primary);
            free_expression(postfix->index.index);
            break;

        default:
            fprintf(stderr, "postfix wrong type: type %u\n", postfix->type);
            exit(EXIT_FAILURE);
    }

    free(postfix);
}


void free_prefix(prefix_t * prefix) {
    switch (prefix->type) {
        case POSTFIX:
            free_postfix(prefix->postfix);
            break;

        case PREFIX:
            free_prefix(prefix->prefix.prefix);
            break;

        default:
            fprintf(stderr, "prefix wrong type: type %u\n", prefix->type);
            exit(EXIT_FAILURE);
    }

    free(prefix);
}


void free_power(power_t * power) {
    switch (power->type) {
        case PREFIX:
            free_prefix(power->prefix);
            break;

        case POWER_:
            free_prefix(power->power.prefix);
            free_power(power->power.power);
            break;

        default:
            fprintf(stderr, "power wrong type: type %u\n", power->type);
            exit(EXIT_FAILURE);
    }

    free(power);
}


void free_multiplication(multiplication_t * multiplication) {
    switch (multiplication->type) {
        case POWER_:
            free_power(multiplication->power);
            break;

        case MULTIPLICATION:
            free_power(multiplication->multiplication.power);
            free_multiplication(multiplication->multiplication.multiplication);
            break;

        default:
            fprintf(stderr, "multiplication wrong type: type %u\n", multiplication->type);
            exit(EXIT_FAILURE);
    }

    free(multiplication);
}


void free_addition(addition_t * addition) {
    switch (addition->type) {
        case MULTIPLICATION:
            free_multiplication(addition->multiplication);
            break;

        case ADDITION:
            free_multiplication(addition->addition.multiplication);
            free_addition(addition->addition.addition);
            break;

        default:
            fprintf(stderr, "addition wrong type: type %u\n", addition->type);
            exit(EXIT_FAILURE);
    }

    free(addition);
}


void free_comparison(comparison_t * comparison) {
    switch (comparison->type) {
        case ADDITION:
            free_addition(comparison->addition);
            break;

        case COMPARISON:
            free_addition(comparison->comparison.addition_lhs);
            free_addition(comparison->comparison.addition_rhs);
            break;

        default:
            fprintf(stderr, "comparison wrong type: type %u\n", comparison->type);
            exit(EXIT_FAILURE);
    }

    free(comparison);
}


void free_expression(expression_t * expression) {
    switch (expression->type) {
        case COMPARISON:
            free_comparison(expression->comparison);
            break;

        case LOGICAL:
            free_comparison(expression->logical.comparison);
            free_expression(expression->logical.expression);
            break;

        default:
            fprintf(stderr, "expression wrong type: type %u\n", expression->type);
            exit(EXIT_FAILURE);
    }

    free(expression);
}


void free_type(type_t * type) {
    switch (type->type) {
        case DATA_TYPE:
            break;
        
        case REFERENCE:
            free_type(type->reference.type);
            break;
        
        default:
            fprintf(stderr, "type wrong type: type %u\n", type->type);
            exit(EXIT_FAILURE);
    }

    free(type);
}


void free_assignment(assignment_t * assignment) {
    free_expression(assignment->expression);
    free(assignment);
}

void free_declaration(declaration_t * declaration) {
    free_type(declaration->type);
    free_assignment(declaration->assignment);
    free(declaration);
}


void free_while(while_t * while_) {
    free_expression(while_->expression);
    free_statement(while_->statement);
    free(while_);
}


void free_for(for_t * for_) {
    free_statement(for_->init);
    free_expression(for_->expression);
    free_statement(for_->end);
    free_statement(for_->body);
    free(for_);
}


void free_if(if_t * if_) {
    switch (if_->type) {
        case IF_ONLY:
            free_expression(if_->if_only.condition);
            free_statement(if_->if_only.body);
            break;
        
        case IF_ELSE:
            free_expression(if_->if_else.condition);
            free_statement(if_->if_else.body);
            free_statement(if_->if_else.else_body);
            break;
        
        default:
            fprintf(stderr, "if wrong type: type %u\n", if_->type);
            exit(EXIT_FAILURE);
    }

    free(if_);
}


void free_function(function_t * function) {
    free_type(function->return_type);
    free_statement(function->body);
    for (struct Parameter * i = function->parameters; i->type != NULL; i++)
        free_type(i->type);
    free(function->parameters);
    free(function);
}


void free_block(block_t * block) {
    for (statement_t ** i = block->statements; *i != NULL; i++)
        free_statement(*i);
    free(block);
}


void free_statement(statement_t * statement) {
    switch (statement->type) {
        case WHILE_STATEMENT:
            free_while(statement->while_);
            break;

        case FOR_STATEMENT:
            free_for(statement->for_);
            break;

        case IF_STATEMENT:
            free_if(statement->if_);
            break;

        case FUNCTION_STATEMENT:
            free_function(statement->function);
            break;

        case BLOCK_STATEMENT:
            free_block(statement->block);
            break;

        case ASSIGNMENT_STATEMENT:
            free_assignment(statement->assignment);
            break;

        case declaration_STATEMENT:
            free_declaration(statement->declaration);
            break;

        case EXPRESSION_STATEMENT:
            free_expression(statement->expression);
            break;

        case RETURN_STATEMENT:
            free_expression(statement->return_statement);
            break;

        case BREAK_STATEMENT:
            break;

        case SKIP_STATEMENT:
            break;
        
        default:
            fprintf(stderr, "statement wrong type: type %u\n", statement->type);
            exit(EXIT_FAILURE);
    }

    free(statement);
}


void free_program(program_t * program) {
    for (statement_t ** i = program->statements; *i != NULL; i++)
        free_statement(*i);
    free(program);
}


void print_primary(primary_t * primary) {
    switch (primary->type) {
        case INTEGER_:
            printf("(int %lu)", primary->integer.start);
            break;
        case FLOATING_POINT_:
            printf("(float %lu)", primary->floating_point.start);
            break;
        case STRING_:
            printf("(str %lu)", primary->string.start);
            break;
        case IDENTIFIER_:
            printf("(id %lu)", primary->identifier.start);
            break;
        case EXPRESSION:
            print_expression(primary->expression);
            break;
        default:
            fprintf(stderr, "primary wrong type: type %u\n", primary->type);
            exit(EXIT_FAILURE);
    }
}


void print_postfix(postfix_t * postfix) {
    switch (postfix->type) {
        case FUNCTION_CALL:
            printf("(funcall ");
            print_primary(postfix->function_call.primary);
            for (expression_t ** i = postfix->function_call.arguments; *i != NULL; i++)
                print_expression(*i);
            printf(")");
            break;

        case INDEX:
            printf("(index ");
            print_primary(postfix->index.primary);
            print_expression(postfix->index.index);
            printf(")");
            break;

        case PRIMARY:
            print_primary(postfix->primary);
            break;

        default:
            fprintf(stderr, "postfix wrong type: type %u\n", postfix->type);
            exit(EXIT_FAILURE);
    }
}


void print_prefix(prefix_t * prefix) {
    switch (prefix->type) {
        case PREFIX:
            printf("(prefix ");
            printf("op:%lu ", prefix->prefix.operator.start);
            print_prefix(prefix->prefix.prefix);
            printf(")");
            break;

        case POSTFIX:
            print_postfix(prefix->postfix);
            break;
        
        default:
            fprintf(stderr, "prefix wrong type: type %u\n", prefix->type);
            exit(EXIT_FAILURE);
    }
}


void print_power(power_t * power) {
    switch (power->type) {
        case POWER_:
            printf("(pow ");
            printf("op:%lu ", power->power.operator.start);
            print_prefix(power->power.prefix);
            print_power(power->power.power);
            printf(")");
            break;

        case PREFIX:
            print_prefix(power->prefix);
            break;
        
        default:
            fprintf(stderr, "power wrong type: type %u\n", power->type);
            exit(EXIT_FAILURE);
    }
}


void print_multiplication(multiplication_t * multiplication) {
    switch (multiplication->type) {
        case MULTIPLICATION:
            printf("(mult ");
            printf("op:%lu ", multiplication->multiplication.operator.start);
            print_power(multiplication->multiplication.power);
            print_multiplication(multiplication->multiplication.multiplication);
            printf(")");
            break;

        case POWER_:
            print_power(multiplication->power);
            break;

        default:
            fprintf(stderr, "multiplication wrong type: type %u\n", multiplication->type);
            exit(EXIT_FAILURE);
    }
}


void print_addition(addition_t * addition) {
    switch (addition->type) {
        case ADDITION:
            printf("(add ");
            printf("op:%lu ", addition->addition.operator.start);
            print_multiplication(addition->addition.multiplication);
            print_addition(addition->addition.addition);
            printf(")");
            break;

        case MULTIPLICATION:
            print_multiplication(addition->multiplication);
            break;
        
        default:
            fprintf(stderr, "addition wrong type: type %u\n", addition->type);
            exit(EXIT_FAILURE);
    }
}


void print_comparison(comparison_t * comparison) {
    switch (comparison->type) {
        case COMPARISON:
            printf("(cmp ");
            printf("op:%lu ", comparison->comparison.operator.start);
            print_addition(comparison->comparison.addition_lhs);
            print_addition(comparison->comparison.addition_rhs);
            printf(")");
            break;

        case ADDITION:
            print_addition(comparison->addition);
            break;
            
        default:
            fprintf(stderr, "comparison wrong type: type %u\n", comparison->type);
            exit(EXIT_FAILURE);
    }
}

void print_expression(expression_t * expression) {
    switch (expression->type) {
        case LOGICAL:
            printf("(logic ");
            printf("op:%lu ", expression->logical.operator.start);
            print_comparison(expression->logical.comparison);
            print_expression(expression->logical.expression);
            printf(")");
            break;

        case COMPARISON:
            print_comparison(expression->comparison);
            break;
        
        default:
            fprintf(stderr, "expression wrong type: type %u\n", expression->type);
            exit(EXIT_FAILURE);
    }
}

void print_type(type_t * type) {
    switch (type->type) {
        case DATA_TYPE:
            printf("(type mut:%u id:%lu)", type->data_type.mut, type->data_type.identifier.start);
            break;
        
        case REFERENCE:
            printf("(ref ");
            printf("mut:%u", type->reference.mut);
            print_type(type->reference.type);
            printf(")");
            break;
        
        default:
            fprintf(stderr, "type wrong type: type %u\n", type->type);
            exit(EXIT_FAILURE);
    }
}


void print_assignment(assignment_t * assignment) {
    printf("(assign ");
    printf("id:%lu ", assignment->identifier.start);
    print_expression(assignment->expression);
    printf(")");
}

void print_declaration(declaration_t * declaration) {
    printf("(decl ");
    print_type(declaration->type);
    print_assignment(declaration->assignment);
    printf(")");
}

void print_while(while_t * while_) {
    printf("(while ");
    print_expression(while_->expression);
    print_statement(while_->statement);
    printf(")");
}


void print_for(for_t * for_) {
    printf("(for ");
    print_statement(for_->init);
    print_expression(for_->expression);
    print_statement(for_->end);
    print_statement(for_->body);
    printf(")");
}


void print_if(if_t * if_) {
    switch (if_->type) {
        case IF_ONLY:
            printf("(if ");
            print_expression(if_->if_only.condition);
            print_statement(if_->if_only.body);
            printf(")");
            break;
        
        case IF_ELSE:
            printf("(if-else ");
            print_expression(if_->if_else.condition);
            print_statement(if_->if_else.body);
            print_statement(if_->if_else.else_body);
            printf(")");
            break;
        
        default:
            fprintf(stderr, "if wrong type: type %u\n", if_->type);
            exit(EXIT_FAILURE);
    }
}


void print_function(function_t * function) {
    printf("(fundec ");
    print_type(function->return_type);
    printf("id:%lu ", function->identifier.start);
    for (struct Parameter * i = function->parameters; i->type != NULL; i++) {
        printf("(param ");
        print_type(i->type);
        printf("id:%lu", i->identifier.start);
        printf(")");
    }
    print_statement(function->body);
    printf(")");
}


void print_block(block_t * block) {
    printf("(block ");
    for (statement_t ** i = block->statements; *i != NULL; i++)
        print_statement(*i);
    printf(")");
}


void print_statement(statement_t * statement) {
    switch (statement->type) {
        case WHILE_STATEMENT:
            print_while(statement->while_);
            break;

        case FOR_STATEMENT:
            print_for(statement->for_);
            break;

        case IF_STATEMENT:
            print_if(statement->if_);
            break;

        case FUNCTION_STATEMENT:
            print_function(statement->function);
            break;

        case BLOCK_STATEMENT:
            print_block(statement->block);
            break;

        case ASSIGNMENT_STATEMENT:
            print_assignment(statement->assignment);
            break;

        case declaration_STATEMENT:
            print_declaration(statement->declaration);
            break;

        case EXPRESSION_STATEMENT:
            print_expression(statement->expression);
            break;

        case RETURN_STATEMENT:
            print_expression(statement->return_statement);
            break;

        case BREAK_STATEMENT:
            printf("(break)");
            break;

        case SKIP_STATEMENT:
            printf("(skip)");
            break;
        
        default:
            fprintf(stderr, "statement wrong type: type %u\n", statement->type);
            exit(EXIT_FAILURE);
    }
}


void print_program(program_t * program) {
    printf("(program ");
    for (statement_t ** i = program->statements; *i != NULL; i++)
        print_statement(*i);
    printf(")");
}