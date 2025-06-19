%{

#include <stdio.h>
#include "parser.tab.h"

int yycolumn = 1;
// executed before every action
#define YY_USER_ACTION          \
        

%}

%option noyywrap
%option yylineno

%%

"&&"                    {return AND;}
"||"                    {return OR;}
"="                     {return ASSIGN;}
"=="                    {return EQUALS;}
"!="                    {return NOT_EQUALS;}
">"                     {return GREATER;}
"<"                     {return LESSER;}
">="                    {return GREATER_EQUALS;}
"<="                    {return LESSER_EQUALS;}
"+"                     {return PLUS;}
"-"                     {return MINUS;}
"*"                     {return PRODUCT;}
"/"                     {return DIVISION;}
"%"                     {return MODULUS;}
"**"                    {return POWER;}
"!"                     {return NOT;}
[0-9]+                  {return INT;}
[0-9]*\.[0-9]+          {return FLOAT;}
\"[^"]*\"               {return STRING;}
"mut"                   {return MUT;}
"ref"                   {return REF;}
"if"                    {return IF;}
"else"                  {return ELSE;}
"while"                 {return WHILE;}
"for"                   {return FOR;}
"stop"                  {return STOP;}
"skip"                  {return SKIP;}
"function"              {return FUNCTION;}
"return"                {return RETURN;}
[a-zA-Z_][0-9a-zA-Z_]*  {return IDENTIFIER;}
"->"                    {return ARROW;}
","                     {return COMMA;}
";"                     {return SEMICOLON;}
"("                     {return BRACKET_OPEN;}
")"                     {return BRACKET_CLOSE;}
"{"                     {return CURLY_BRACKET_OPEN;}
"}"                     {return CURLY_BRACKET_CLOSE;}
"["                     {return SQR_BRACKET_OPEN;}
"]"                     {return SQR_BRACKET_CLOSE;}
[\t ]                   {;}
\n                      {}
"//"[^\n]*              {;}
.                       {return UNKNOWN;}

%%