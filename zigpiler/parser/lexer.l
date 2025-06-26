%{

#include "parser.h"
#include "parsetree.h"

int yycolumn = 1;
// executed before every action
#define YY_USER_ACTION              \
    start_line = prev_yylineno;     \
    start_column = yycolumn;        \
    if (yylineno == prev_yylineno)  \
        yycolumn += yyleng;         \
    else                            \
        yycolumn = 1;

%}

%option 8bit
%option warn nodefault
%option yylineno noyywrap
%option outfile="lexer.c"

%%
    size_t start_line, start_column;
    size_t prev_yylineno = yylineno;

"&&"                    {yylval.token = new_token(start_line, start_column, yytext, AND); return AND;}
"||"                    {yylval.token = new_token(start_line, start_column, yytext, OR); return OR;}
"="                     {yylval.token = new_token(start_line, start_column, yytext, ASSIGN); return ASSIGN;}
"=="                    {yylval.token = new_token(start_line, start_column, yytext, EQUALS); return EQUALS;}
"!="                    {yylval.token = new_token(start_line, start_column, yytext, NOT_EQUALS); return NOT_EQUALS;}
">"                     {yylval.token = new_token(start_line, start_column, yytext, GREATER); return GREATER;}
"<"                     {yylval.token = new_token(start_line, start_column, yytext, LESSER); return LESSER;}
">="                    {yylval.token = new_token(start_line, start_column, yytext, GREATER_EQUALS); return GREATER_EQUALS;}
"<="                    {yylval.token = new_token(start_line, start_column, yytext, LESSER_EQUALS); return LESSER_EQUALS;}
"+"                     {yylval.token = new_token(start_line, start_column, yytext, PLUS); return PLUS;}
"-"                     {yylval.token = new_token(start_line, start_column, yytext, MINUS); return MINUS;}
"*"                     {yylval.token = new_token(start_line, start_column, yytext, PRODUCT); return PRODUCT;}
"/"                     {yylval.token = new_token(start_line, start_column, yytext, DIVISION); return DIVISION;}
"%"                     {yylval.token = new_token(start_line, start_column, yytext, MODULUS); return MODULUS;}
"**"                    {yylval.token = new_token(start_line, start_column, yytext, POWER); return POWER;}
"!"                     {yylval.token = new_token(start_line, start_column, yytext, NOT); return NOT;}
[0-9]+                  {yylval.token = new_token(start_line, start_column, yytext, INT); return INT;}
[0-9]*\.[0-9]+          {yylval.token = new_token(start_line, start_column, yytext, FLOAT); return FLOAT;}
\"[^"\n]*\"             {yylval.token = new_token(start_line, start_column, yytext, STRING); return STRING;}
"mut"                   {yylval.token = new_token(start_line, start_column, yytext, MUT); return MUT;}
"ref"                   {yylval.token = new_token(start_line, start_column, yytext, REF); return REF;}
"if"                    {yylval.token = new_token(start_line, start_column, yytext, IF); return IF;}
"else"                  {yylval.token = new_token(start_line, start_column, yytext, ELSE); return ELSE;}
"while"                 {yylval.token = new_token(start_line, start_column, yytext, WHILE); return WHILE;}
"for"                   {yylval.token = new_token(start_line, start_column, yytext, FOR); return FOR;}
"stop"                  {yylval.token = new_token(start_line, start_column, yytext, STOP); return STOP;}
"skip"                  {yylval.token = new_token(start_line, start_column, yytext, SKIP); return SKIP;}
"function"              {yylval.token = new_token(start_line, start_column, yytext, FUNCTION); return FUNCTION;}
"return"                {yylval.token = new_token(start_line, start_column, yytext, RETURN); return RETURN;}
[a-zA-Z_][0-9a-zA-Z_]*  {yylval.token = new_token(start_line, start_column, yytext, IDENTIFIER); return IDENTIFIER;}
"->"                    {yylval.token = new_token(start_line, start_column, yytext, ARROW); return ARROW;}
","                     {yylval.token = new_token(start_line, start_column, yytext, COMMA); return COMMA;}
";"                     {yylval.token = new_token(start_line, start_column, yytext, SEMICOLON); return SEMICOLON;}
"("                     {yylval.token = new_token(start_line, start_column, yytext, BRACKET_OPEN); return BRACKET_OPEN;}
")"                     {yylval.token = new_token(start_line, start_column, yytext, BRACKET_CLOSE); return BRACKET_CLOSE;}
"{"                     {yylval.token = new_token(start_line, start_column, yytext, CURLY_BRACKET_OPEN); return CURLY_BRACKET_OPEN;}
"}"                     {yylval.token = new_token(start_line, start_column, yytext, CURLY_BRACKET_CLOSE); return CURLY_BRACKET_CLOSE;}
"["                     {yylval.token = new_token(start_line, start_column, yytext, SQR_BRACKET_OPEN); return SQR_BRACKET_OPEN;}
"]"                     {yylval.token = new_token(start_line, start_column, yytext, SQR_BRACKET_CLOSE); return SQR_BRACKET_CLOSE;}
[\t ]                   {;}
\n                      {;}
"//"[^\n]*              {;}
.                       {yylval.token = new_token(start_line, start_column, yytext, YYerror); return YYerror;}

%%