%{
    #include <stdio.h>

    int yylex(void);
    void yyerror(char const *s);
%}

// operators*
%token AND
%token OR
%token ASSIGN
%token EQUALS
%token NOT_EQUALS
%token GREATER
%token LESSER
%token GREATER_EQUALS
%token LESSER_EQUALS
%token PLUS
%token MINUS
%token PRODUCT
%token DIVISION
%token MODULUS
%token POWER
%token NOT

// literals
%token INT
%token FLOAT
%token STRING

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
%token IDENTIFIER

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

// error
%token UNKNOWN

// operator precedence & shit
%left AND OR
// %left ASSIGN
%nonassoc EQUALS NOT_EQUALS
%nonassoc GREATER LESSER GREATER_EQUALS LESSER_EQUALS
%left PLUS MINUS
%left PRODUCT DIVISION MODULUS
%left POWER
%precedence NOT

%%

start: statement start
    | %empty
    ;

statement: expression SEMICOLON
    | SEMICOLON
    ;

expression: INT
    | FLOAT
    | STRING
    | IDENTIFIER
    | expression AND expression
    | expression OR expression
    | expression EQUALS expression
    | expression NOT_EQUALS expression
    | expression GREATER expression
    | expression LESSER expression
    | expression GREATER_EQUALS expression
    | expression LESSER_EQUALS expression
    | expression PLUS expression
    | expression MINUS expression
    | expression PRODUCT expression
    | expression DIVISION expression
    | expression MODULUS expression
    | expression POWER expression
    | NOT expression 
    | BRACKET_OPEN expression BRACKET_CLOSE
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