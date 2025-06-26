%{
    #include <stdio.h>
    #include "parsetree.h"

    int yylex();
    void yyerror(char const *s);
%}

%output "parser.c"
%header "parser.h"

%union {
    struct token * token;
    struct binary_operator * binary_operator;
    struct unary_operator * unary_operator;
    struct expression * expression;
}

// operators*
%token <token> AND
%token <token> OR
%token <token> ASSIGN
%token <token> EQUALS
%token <token> NOT_EQUALS
%token <token> GREATER
%token <token> LESSER
%token <token> GREATER_EQUALS
%token <token> LESSER_EQUALS
%token <token> PLUS
%token <token> MINUS
%token <token> PRODUCT
%token <token> DIVISION
%token <token> MODULUS
%token <token> POWER
%token <token> NOT

// literals
%token <token> INT
%token <token> FLOAT
%token <token> STRING

// alpha
%token MUT
%token REF
%token IF
%token ELSE
%token WHILE
%token FOR
%token STOP
%token SKIP
%token FUNCTION
%token RETURN
%token <token> IDENTIFIER

// syntax
%token ARROW
%token COMMA
%token SEMICOLON
%token BRACKET_OPEN
%token BRACKET_CLOSE
%token CURLY_BRACKET_OPEN
%token CURLY_BRACKET_CLOSE
%token SQR_BRACKET_OPEN
%token SQR_BRACKET_CLOSE


// operator precedence & shit
%left AND OR
// %left ASSIGN
%nonassoc EQUALS NOT_EQUALS
%nonassoc GREATER LESSER GREATER_EQUALS LESSER_EQUALS
%left PLUS MINUS
%left PRODUCT DIVISION MODULUS
%left POWER
%precedence NOT

// types of non-terminals
%type <expression> expression

%%

start: statement start  { }
    | %empty            { }
    ;

statement: expression SEMICOLON { print_expression($1); }
    | SEMICOLON                 { }
    ;

expression: INT                             { $$ = new_value_expression($1); }
    | FLOAT                                 { $$ = new_value_expression($1); }
    | STRING                                { $$ = new_value_expression($1); }
    | IDENTIFIER                            { $$ = new_value_expression($1); }
    | expression AND expression             { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression OR expression              { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression EQUALS expression          { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression NOT_EQUALS expression      { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression GREATER expression         { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression LESSER expression          { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression GREATER_EQUALS expression  { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression LESSER_EQUALS expression   { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression PLUS expression            { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression MINUS expression           { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression PRODUCT expression         { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression DIVISION expression        { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression MODULUS expression         { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | expression POWER expression           { $$ = new_binary_operator_expression(new_binary_operator($2, $1, $3)); }
    | NOT expression                        { $$ = new_unary_operator_expression(new_unary_operator($1, $2)); }
    | BRACKET_OPEN expression BRACKET_CLOSE { $$ = $2; }
    ;

%%

void yyerror(char const *s) {
    printf("%s",s);
}

int main()
{
	yyparse();
	return 1;
}