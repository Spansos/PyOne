#include "parser.h"

#include <stdlib.h>

#include <stdio.h>

const char * error_type_string(parse_error_t error) {
    switch (error.type) {
        case EXPECTED_OPEN_BRACKET:
            return "expected_open_bracket";
        case EXPECTED_OPEN_CURLY_BRACKET:
            return "expected_open_curly_bracket";
        case EXPECTED_CLOSING_SQUARE_BRACKET:
            return "expected_closing_square_bracket";
        case EXPECTED_CLOSING_BRACKET:
            return "expected_closing_bracket";
        case EXPECTED_EXPRESSION:
            return "expected_expression";
        case EXPECTED_TYPE:
            return "expected_type";
        case EXPECTED_ASSIGNMENT:
            return "expected_assignment";
        case EXPECTED_IDENTIFIER:
            return "expected_identifier";
        case EXPECTED_WHILE:
            return "expected_while";
        case EXPECTED_FOR:
            return "expected_for";
        case EXPECTED_IF:
            return "expected_if";
        case EXPECTED_FUNCTION:
            return "expected_function";
        case EXPECTED_SEMICOLON:
            return "expected_semicolon";
        case EXPECTED_ARROW:
            return "expected_arrow";
    }

    return "Invalid error";
}


expression_t * parse_expression(token_array_t * tokens, parse_error_t * error);
statement_t * parse_statement(token_array_t * tokens, parse_error_t * error);

primary_t * parse_primary(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    primary_t * primary = malloc(sizeof(primary_t));
    
    token_t token = tokens->tokens[0];
    switch (token.type) {
        case INT:
            tokens->tokens++;
            primary->type = INTEGER_;
            primary->integer = token;
            break;

        case FLOAT:
            tokens->tokens++;
            primary->type = FLOATING_POINT_;
            primary->floating_point = token;
            break;

        case STRING:
            tokens->tokens++;
            primary->type = STRING_;
            primary->string = token;
            break;

        case IDENTIFIER:
            tokens->tokens++;
            primary->type = IDENTIFIER_;
            primary->identifier = token;
            break;

        case BRACKET_OPEN:
            tokens->tokens++;
            primary->type = EXPRESSION;
            // parse expression
            primary->expression = parse_expression(tokens, error);
            if (!primary->expression) {
                free(primary);
                tokens->tokens = start;
                return NULL;
            }
            // check closing bracket
            if (tokens->tokens[0].type != BRACKET_CLOSE) {
                // cleanup
                free_primary(primary);
                tokens->tokens = start;
                // error
                error->token = tokens->tokens[0];
                error->type = EXPECTED_CLOSING_BRACKET;
                return NULL;
            }
            tokens->tokens++;
            break;

        default:
            error->token = token;
            error->type = EXPECTED_EXPRESSION;
            free(primary);
            return NULL;
    }

    return primary;
}

postfix_t * parse_postfix(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    // parse primary
    primary_t * primary = parse_primary(tokens, error);
    if (!primary)
        return NULL;

    // make postfix
    postfix_t * postfix = malloc(sizeof(postfix_t));

    // construct postifx depending on next tokens
    switch (tokens->tokens[0].type) {
        // parse function call
        case BRACKET_OPEN:
            // skip bracket
            tokens->tokens++;
            // awwauuh
            postfix->type = FUNCTION_CALL;
            postfix->function_call.primary = primary;

            // parse arguments
            // realloc maybe should be optimized to not resize for every new arg
            size_t arg_count = 0;
            postfix->function_call.arguments = calloc(1, sizeof(expression_t*));

            // while function arg parsing
            while (tokens->tokens[0].type != BRACKET_CLOSE) {
                // check comma if not first argument
                if (arg_count != 0) {
                    if (tokens->tokens[0].type != COMMA) {
                        free_postfix(postfix);
                        error->token = tokens->tokens[0];
                        error->type = EXPECTED_CLOSING_BRACKET;
                        tokens->tokens = start;
                        return NULL;
                    }
                    tokens->tokens++;
                }
                // new arg
                arg_count++;
                postfix->function_call.arguments = realloc(postfix->function_call.arguments, (arg_count+1)*sizeof(expression_t*));
                // parse arg
                expression_t * arg = parse_expression(tokens, error);
                if (!arg) {
                    free_postfix(postfix);
                    tokens->tokens = start;
                    return NULL;
                }
                // assign to arguments array
                postfix->function_call.arguments[arg_count-1] = arg;
                postfix->function_call.arguments[arg_count] = NULL;
            }
            tokens->tokens++;

            return postfix;

        // parse index
        case SQR_BRACKET_OPEN:
            // skip bracket
            tokens->tokens++;

            // parse index
            postfix->type = INDEX;
            postfix->index.primary = primary;
            postfix->index.index = parse_expression(tokens, error);

            // error handling
            // (index parsing went wrong)
            if (!postfix->index.index) {
                free(postfix);
                free_primary(primary);
                tokens->tokens = start;
                return NULL;
            }
            // (no closing bracket)
            if (tokens->tokens[0].type != SQR_BRACKET_CLOSE) {
                free_postfix(postfix);
                error->token = tokens->tokens[0];
                error->type = EXPECTED_CLOSING_SQUARE_BRACKET;
                tokens->tokens = start;
                return NULL;
            }

            return postfix;

        // just return primary
        default:
            postfix->type = PRIMARY;
            postfix->primary = primary;
            return postfix;
    }
}


prefix_t * parse_prefix(token_array_t * tokens, parse_error_t * error)
{
    // if next token == MINUS | NOT | PLUS
    token_t next_token = tokens->tokens[0];
    token_type_t next_type = next_token.type;
    if (next_type == MINUS || next_type == NOT || next_type == PLUS) {
        tokens->tokens++;

        prefix_t * prefix = parse_prefix(tokens, error);
        if (!prefix) {
            tokens->tokens--;
            return NULL;
        }

        // return prefix
        prefix_t * prefix_ = malloc(sizeof(prefix_t));

        prefix_->type = PREFIX;
        prefix_->prefix.prefix = prefix;
        prefix_->prefix.operator = next_token;
        
        return prefix_;
    }

    // return postfix
    postfix_t * postfix = parse_postfix(tokens, error);
    if (!postfix)
        return NULL;

    prefix_t * prefix = malloc(sizeof(prefix_t));

    prefix->type = POSTFIX;
    prefix->postfix = postfix;
    
    return prefix;
}


power_t * parse_power(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    // parse rhs
    prefix_t * lhs = parse_prefix(tokens, error);
    if (!lhs)
        return NULL;
    

    // if next token == POWER
    token_t next_token = tokens->tokens[0];
    token_type_t next_type = next_token.type;
    if (next_type == POWER) {
        tokens->tokens++;

        power_t * rhs = parse_power(tokens, error);
        if (!rhs) {
            tokens->tokens = start;
            free_prefix(lhs);
            return NULL;
        }

        // return power
        power_t * power = malloc(sizeof(power_t));

        power->type = POWER_;
        power->power.prefix = lhs;
        power->power.power = rhs;
        power->power.operator = next_token;
        
        return power;
    }

    // return prefix
    power_t * power = malloc(sizeof(power_t));

    power->type = PREFIX;
    power->prefix = lhs;
    
    return power;
}


multiplication_t * parse_multiplication(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    // parse rhs
    power_t * lhs = parse_power(tokens, error);
    if (!lhs)
        return NULL;
    

    // if next token == MULT|DIVISION|MODULUS
    token_t next_token = tokens->tokens[0];
    token_type_t next_type = next_token.type;
    if (next_type == MULT || next_type == DIVISION || next_type == MODULUS) {
        tokens->tokens++;

        multiplication_t * rhs = parse_multiplication(tokens, error);
        if (!rhs) {
            tokens->tokens = start;
            free_power(lhs);
            return NULL;
        }

        // return multiplication
        multiplication_t * multiplication = malloc(sizeof(multiplication_t));

        multiplication->type = MULTIPLICATION;
        multiplication->multiplication.power = lhs;
        multiplication->multiplication.multiplication = rhs;
        multiplication->multiplication.operator = next_token;
        
        return multiplication;
    }

    // return power
    multiplication_t * multiplication = malloc(sizeof(multiplication_t));

    multiplication->type = POWER_;
    multiplication->power = lhs;
    
    return multiplication;
}


addition_t * parse_addition(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    // parse rhs
    multiplication_t * lhs = parse_multiplication(tokens, error);
    if (!lhs)
        return NULL;

    // if next token == MINUS|PLUS
    token_t next_token = tokens->tokens[0];
    token_type_t next_type = next_token.type;
    if (next_type == MINUS || next_type == PLUS) {
        tokens->tokens++;

        addition_t * rhs = parse_addition(tokens, error);
        if (!rhs) {
            tokens->tokens = start;
            free_multiplication(lhs);
            return NULL;
        }

        // return addition
        addition_t * addition = malloc(sizeof(addition_t));

        addition->type = ADDITION;
        addition->addition.addition = rhs;
        addition->addition.multiplication = lhs;
        addition->addition.operator = next_token;
        
        return addition;
    }

    // return multiplication
    addition_t * addition = malloc(sizeof(addition_t));

    addition->type = MULTIPLICATION;
    addition->multiplication = lhs;
    
    return addition;
}


// parse a comparison
comparison_t * parse_comparison(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    // parse lhs
    addition_t * lhs = parse_addition(tokens, error);
    if (!lhs)
        return NULL;

    // if next token == EQUALS|NOT_EQUALS|GREATER_EQUAL|LESSER_EQUAL|GREATER|LESSER
    token_t next_token = tokens->tokens[0];
    token_type_t next_type = next_token.type;
    if (next_type == EQUALS || next_type == NOT_EQUALS || next_type == GREATER_EQUAL || next_type == LESSER_EQUAL || next_type == GREATER || next_type == LESSER) {
        tokens->tokens++;

        addition_t * rhs = parse_addition(tokens, error);
        if (!rhs) {
            tokens->tokens = start;
            free_addition(lhs);
            return NULL;
        }

        // return comparison
        comparison_t * comparison = malloc(sizeof(comparison_t));

        comparison->type = COMPARISON;
        comparison->comparison.addition_lhs = lhs;
        comparison->comparison.addition_rhs = rhs;
        comparison->comparison.operator = next_token;

        return comparison;
    }
    
    comparison_t * comparison = malloc(sizeof(comparison_t));

    comparison->type = ADDITION;
    comparison->addition = lhs;

    return comparison;
}


// parse an expression
expression_t * parse_expression(token_array_t * tokens, parse_error_t * error)
{
    token_t * start = tokens->tokens;

    // parse lhs
    comparison_t * lhs = parse_comparison(tokens, error);
    if (!lhs)
        return NULL;

    // if next token == AND or OR, do rhs
    if (tokens->tokens[0].type == AND || tokens->tokens[0].type == OR) {
        token_t token = tokens->tokens[0];
        tokens->tokens++;

        expression_t * rhs = parse_expression(tokens, error);
        if (!rhs) {
            tokens->tokens = start;
            free_comparison(lhs);
            return NULL;
        }
        
        // return logical expression
        expression_t * expression = malloc(sizeof(expression_t));

        expression->type = LOGICAL;
        expression->logical.operator = token;
        expression->logical.comparison = lhs;
        expression->logical.expression = rhs;

        return expression;
    }

    // return comparison expression
    expression_t * expression = malloc(sizeof(expression_t));

    expression->type = COMPARISON;
    expression->comparison = lhs;

    return expression;
}


type_t * parse_type(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // reference
    bool is_mut;
    if (tokens->tokens[0].type == MUT) {
        is_mut = true;
        tokens->tokens++;
    } else {
        is_mut = false;
    }

    token_t token = tokens->tokens[0];
    type_t * type;
    switch (token.type) {
        // identifier of datatype
        case IDENTIFIER:
            tokens->tokens++;

            type = malloc(sizeof(type_t));

            type->type = DATA_TYPE;
            type->data_type.identifier = token;
            type->data_type.mut = is_mut;

            return type;
        
        // pointer to type
        case REF:; // 'a label can only be part of a statement and a declaration is not a statement' - ☝️🤓
            // parse sub-type
            type_t * ref_type = parse_type(tokens, error);
            if (!ref_type)
                return NULL;

            // construct that thang
            type = malloc(sizeof(type_t));

            type->type = REFERENCE;
            type->reference.mut = is_mut;
            type->reference.type = ref_type;

            return type;
            
        // uwaaawa uuhh errror
        default:
            error->type = EXPECTED_TYPE;
            error->token = token;
            tokens->tokens = start;
            return NULL;
    }

}

assignment_t * parse_assignment(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // identifier
    token_t id = tokens->tokens[0];
    if (id.type != IDENTIFIER) {
        error->token = id;
        error->type = EXPECTED_IDENTIFIER;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // assign
    token_t assign = tokens->tokens[0];
    if (assign.type != ASSIGN) {
        error->token = assign;
        error->type = EXPECTED_ASSIGNMENT;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // expression
    expression_t * expression = parse_expression(tokens, error);
    
    if (!expression) {
        tokens->tokens = start;
        return NULL;
    }
    
    // construct & return
    assignment_t * assignment = malloc(sizeof(assignment_t));
    
    assignment->expression = expression;
    assignment->identifier = id;
    
    return assignment;
}


declaration_t * parse_declaration(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // type
    type_t * type = parse_type(tokens, error);
    if (!type)
        return NULL;
    
    // assignment
    assignment_t * assignment = parse_assignment(tokens, error);
    if (!assignment) {
        free_type(type);
        tokens->tokens = start;
        return NULL;
    }

    declaration_t * declaration = malloc(sizeof(declaration_t));

    declaration->type = type;
    declaration->assignment = assignment;

    return declaration;
}


while_t * parse_while(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // while
    token_t while_ = tokens->tokens[0];
    if (while_.type != WHILE) {
        error->token = while_;
        error->type = EXPECTED_WHILE;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // open bracket
    token_t open_bracket = tokens->tokens[0];
    if (open_bracket.type != BRACKET_OPEN) {
        error->token = open_bracket;
        error->type = EXPECTED_OPEN_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // expression
    expression_t * expression = parse_expression(tokens, error);
    if (!expression) {
        tokens->tokens = start;
        return NULL;
    }

    // closing bracket
    token_t close_bracket = tokens->tokens[0];
    if (close_bracket.type != BRACKET_CLOSE) {
        free_expression(expression);
        error->token = close_bracket;
        error->type = EXPECTED_CLOSING_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // statement
    statement_t * statement = parse_statement(tokens, error);
    if (!statement) {
        free_expression(expression);
        tokens->tokens = start;
        return NULL;
    }
    
    // construct & return
    while_t * while__ = malloc(sizeof(while_t));

    while__->expression = expression;
    while__->statement = statement;

    return while__;
}


for_t * parse_for(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // for
    token_t for_ = tokens->tokens[0];
    if (for_.type != FOR) {
        error->token = for_;
        error->type = EXPECTED_FOR;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // open bracket
    token_t open_bracket = tokens->tokens[0];
    if (open_bracket.type != BRACKET_OPEN) {
        error->token = open_bracket;
        error->type = EXPECTED_OPEN_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // statement
    statement_t * statement1 = parse_statement(tokens, error);
    if (!statement1) {
        tokens->tokens = start;
        return NULL;
    }

    // expression
    expression_t * expression = parse_expression(tokens, error);
    if (!expression) {
        free_statement(statement1);
        tokens->tokens = start;
        return NULL;
    }

    // semi-colon
    if (tokens->tokens[0].type != SEMICOLON) {
        free_statement(statement1);
        free_expression(expression);
        error->token = tokens->tokens[0];
        error->type = EXPECTED_SEMICOLON;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // statement
    statement_t * statement2 = parse_statement(tokens, error);
    if (!statement2) {
        free_statement(statement1);
        free_expression(expression);
        tokens->tokens = start;
        return NULL;
    }

    // closing bracket
    token_t close_bracket = tokens->tokens[0];
    if (close_bracket.type != BRACKET_CLOSE) {
        free_statement(statement1);
        free_expression(expression);
        free_statement(statement2);
        error->token = close_bracket;
        error->type = EXPECTED_CLOSING_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // statement
    statement_t * body = parse_statement(tokens, error);
    if (!body) {
        free_statement(statement1);
        free_expression(expression);
        free_statement(statement2);
        tokens->tokens = start;
        return NULL;
    }
    
    // construct & return
    for_t * for__ = malloc(sizeof(for_t));

    for__->init = statement1;
    for__->expression = expression;
    for__->end = statement2;
    for__->body = body;

    return for__;
}


if_t * parse_if(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // if
    if (tokens->tokens[0].type != IF) {
        error->token = tokens->tokens[0];
        error->type = EXPECTED_IF;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // open bracket
    if (tokens->tokens[0].type != BRACKET_OPEN) {
        error->token = tokens->tokens[0];
        error->type = EXPECTED_OPEN_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // expression
    expression_t * expression = parse_expression(tokens, error);
    if (!expression) {
        tokens->tokens = start;
        return NULL;
    }

    // closing bracket
    if (tokens->tokens[0].type != BRACKET_CLOSE) {
        free_expression(expression);
        error->token = tokens->tokens[0];
        error->type = EXPECTED_CLOSING_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // statement
    statement_t * statement = parse_statement(tokens, error);
    if (!statement) {
        free_expression(expression);
        tokens->tokens = start;
        return NULL;
    }

    // else
    if (tokens->tokens[0].type == ELSE) {
        tokens->tokens++;

        statement_t * else_body = parse_statement(tokens, error);
        if (!else_body) {
            free_expression(expression);
            free_statement(statement);
            tokens->tokens = start;
            return NULL;
        }

        if_t * if_ = malloc(sizeof(if_t));

        if_->type = IF_ELSE;
        if_->if_else.condition = expression;
        if_->if_else.body = statement;
        if_->if_else.else_body = else_body;

        return if_;
    }
    
    if_t * if_ = malloc(sizeof(if_t));

    if_->type = IF_ONLY;
    if_->if_only.condition = expression;
    if_->if_only.body = statement;

    return if_;
}


function_t * parse_function(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // funcitoon
    if (tokens->tokens[0].type != FUNCTION) {
        error->token = tokens->tokens[0];
        error->type = EXPECTED_FUNCTION;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // identifier
    token_t id = tokens->tokens[0];
    if (id.type != IDENTIFIER) {
        error->token = id;
        error->type = EXPECTED_IDENTIFIER;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // open bracket
    if (tokens->tokens[0].type != BRACKET_OPEN) {
        error->token = tokens->tokens[0];
        error->type = EXPECTED_OPEN_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    // parameters
    struct Parameter * parameters = calloc(sizeof(struct Parameter), 1);
    size_t param_count = 0;
    while (tokens->tokens[0].type != BRACKET_CLOSE) {
        // check comma if not first argument
        if (param_count != 0) {
            if (tokens->tokens[0].type != COMMA) {
                for (struct Parameter * i = parameters; i->type != NULL; i++)
                    free_type(i->type);
                free(parameters);
                error->token = tokens->tokens[0];
                error->type = EXPECTED_CLOSING_BRACKET;
                tokens->tokens = start;
                return NULL;
            }
            tokens->tokens++;
        }

        // new param
        param_count++;
        parameters = realloc(parameters, (param_count+1)*sizeof(struct Parameter));

        // parse type & identifrier
        type_t * type = parse_type(tokens, error);
        if (!type) {
            for (struct Parameter * i = parameters; i->type != NULL; i++)
                free_type(i->type);
            free(parameters);
            tokens->tokens = start;
            return NULL;
        }
        // assign to arguments array
        parameters[param_count-1].type = type;
        parameters[param_count].type = NULL;

        parameters[param_count-1].identifier = tokens->tokens[0];
        if (parameters[param_count-1].identifier.type != IDENTIFIER) {
            error->type = EXPECTED_IDENTIFIER;
            error->token = parameters[param_count-1].identifier;
            for (struct Parameter * i = parameters; i->type != NULL; i++)
                free_type(i->type);
            free(parameters);
            tokens->tokens = start;
            return NULL;
        }
        tokens->tokens++;
    }
    // closing bracket
    tokens->tokens++;

    // arrow
    if (tokens->tokens[0].type != ARROW) {
        error->token = tokens->tokens[0];
        error->type = EXPECTED_ARROW;
        tokens->tokens = start;
        for (struct Parameter * i = parameters; i->type != NULL; i++)
            free_type(i->type);
        free(parameters);
        return NULL;
    }
    tokens->tokens++;

    // type
    type_t * type = parse_type(tokens, error);
    if (!type) {
        for (struct Parameter * i = parameters; i->type != NULL; i++)
            free_type(i->type);
        free(parameters);
        tokens->tokens = start;
        return NULL;
    }

    // body
    statement_t * body = parse_statement(tokens, error);
    if (!body) {
        for (struct Parameter * i = parameters; i->type != NULL; i++)
            free_type(i->type);
        free(parameters);
        free_type(type);
        tokens->tokens = start;
        return NULL;
    }

    // construct & return
    function_t * function = malloc(sizeof(function_t));

    function->identifier = id;
    function->return_type = type;
    function->parameters = parameters;
    function->body = body;

    return function;
}


block_t * parse_block(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    // open curled bracketuq
    if (tokens->tokens[0].type != CURLY_BRACKET_OPEN) {
        error->token = tokens->tokens[0];
        error->type = EXPECTED_OPEN_CURLY_BRACKET;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    block_t * block = malloc(sizeof(block_t));
    
    block->statements = calloc(sizeof(statement_t*), 1);
    size_t statement_count = 0;
    while (tokens->tokens[0].type != CURLY_BRACKET_CLOSE) {
        statement_t * statement = parse_statement(tokens, error);
        if (!statement) {
            free_block(block);
            tokens->tokens = start;
            return NULL;
        }
        
        statement_count++;
        block->statements = realloc(block->statements, (statement_count+1)*sizeof(statement_t*));
        
        block->statements[statement_count-1] = statement;
        block->statements[statement_count] = NULL;
    }
    tokens->tokens++;

    return block;
}


statement_t * parse_statement(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    statement_t * statement = malloc(sizeof(statement_t));

    switch (tokens->tokens[0].type) {
        case WHILE:
            statement->type = WHILE_STATEMENT;
            statement->while_ = parse_while(tokens, error);
            if (!statement->while_) {
                free(statement);
                return NULL;
            }
            return statement;

        case FOR:
            statement->type = FOR_STATEMENT;
            statement->for_ = parse_for(tokens, error);
            if (!statement->for_) {
                free(statement);
                return NULL;
            }
            return statement;

        case IF:
            statement->type = IF_STATEMENT;
            statement->if_ = parse_if(tokens, error);
            if (!statement->if_) {
                free(statement);
                return NULL;
            }
            return statement;

        case FUNCTION:
            statement->type = FUNCTION_STATEMENT;
            statement->function = parse_function(tokens, error);
            if (!statement->function) {
                free(statement);
                return NULL;
            }
            return statement;

        case CURLY_BRACKET_OPEN:
            statement->type = BLOCK_STATEMENT;
            statement->block = parse_block(tokens, error);
            if (!statement->block) {
                free(statement);
                return NULL;
            }
            return statement;

        case RETURN:
            tokens->tokens++;
            statement->type = RETURN_STATEMENT;
            statement->return_statement = parse_expression(tokens, error);
            if (!statement->return_statement) {
                free(statement);
                return NULL;
            }
            break;

        case BREAK:
            tokens->tokens++;
            statement->type = BREAK_STATEMENT;
            break;

        case SKIP:
            tokens->tokens++;
            statement->type = SKIP_STATEMENT;
            break;

        default:;
            declaration_t * declaration = parse_declaration(tokens, error);
            if (declaration) {
                statement->type = declaration_STATEMENT;
                statement->declaration = declaration;
                break;
            }

            parse_error_t temp_error;
            assignment_t * assignment = parse_assignment(tokens, &temp_error);
            if (assignment) {
                statement->type = ASSIGNMENT_STATEMENT;
                statement->assignment = assignment;
                break;
            }
            *error = temp_error.token.start > error->token.start ? temp_error : *error;

            expression_t * expression = parse_expression(tokens, &temp_error);

            if (expression) {
                statement->type = EXPRESSION_STATEMENT;
                statement->expression = expression;
                break;
            }
            *error = temp_error.token.start > error->token.start ? temp_error : *error;

            free(statement);
            return NULL;
    }

    // semicolon
    if (tokens->tokens[0].type != SEMICOLON) {
        free_statement(statement);
        error->token = tokens->tokens[0];
        error->type = EXPECTED_SEMICOLON;
        tokens->tokens = start;
        return NULL;
    }
    tokens->tokens++;

    return statement;
}


program_t * parse_program(token_array_t * tokens, parse_error_t * error) {
    token_t * start = tokens->tokens;

    program_t * program = malloc(sizeof(program_t));
    
    program->statements = calloc(sizeof(statement_t*), 1);
    size_t statement_count = 0;
    while (tokens->tokens[0].type != INVALID) {
        statement_t * statement = parse_statement(tokens, error);
        if (!statement) {
            free_program(program);
            tokens->tokens = start;
            return NULL;
        }
        
        statement_count++;
        program->statements = realloc(program->statements, (statement_count+1)*sizeof(statement_t*));
        
        program->statements[statement_count-1] = statement;
        program->statements[statement_count] = NULL;
    }

    tokens->tokens = start;
    return program;
}