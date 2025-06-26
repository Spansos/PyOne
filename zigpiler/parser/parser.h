/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    AND = 258,                     /* AND  */
    OR = 259,                      /* OR  */
    ASSIGN = 260,                  /* ASSIGN  */
    EQUALS = 261,                  /* EQUALS  */
    NOT_EQUALS = 262,              /* NOT_EQUALS  */
    GREATER = 263,                 /* GREATER  */
    LESSER = 264,                  /* LESSER  */
    GREATER_EQUALS = 265,          /* GREATER_EQUALS  */
    LESSER_EQUALS = 266,           /* LESSER_EQUALS  */
    PLUS = 267,                    /* PLUS  */
    MINUS = 268,                   /* MINUS  */
    PRODUCT = 269,                 /* PRODUCT  */
    DIVISION = 270,                /* DIVISION  */
    MODULUS = 271,                 /* MODULUS  */
    POWER = 272,                   /* POWER  */
    NOT = 273,                     /* NOT  */
    INT = 274,                     /* INT  */
    FLOAT = 275,                   /* FLOAT  */
    STRING = 276,                  /* STRING  */
    MUT = 277,                     /* MUT  */
    REF = 278,                     /* REF  */
    IF = 279,                      /* IF  */
    ELSE = 280,                    /* ELSE  */
    WHILE = 281,                   /* WHILE  */
    FOR = 282,                     /* FOR  */
    STOP = 283,                    /* STOP  */
    SKIP = 284,                    /* SKIP  */
    FUNCTION = 285,                /* FUNCTION  */
    RETURN = 286,                  /* RETURN  */
    IDENTIFIER = 287,              /* IDENTIFIER  */
    ARROW = 288,                   /* ARROW  */
    COMMA = 289,                   /* COMMA  */
    SEMICOLON = 290,               /* SEMICOLON  */
    BRACKET_OPEN = 291,            /* BRACKET_OPEN  */
    BRACKET_CLOSE = 292,           /* BRACKET_CLOSE  */
    CURLY_BRACKET_OPEN = 293,      /* CURLY_BRACKET_OPEN  */
    CURLY_BRACKET_CLOSE = 294,     /* CURLY_BRACKET_CLOSE  */
    SQR_BRACKET_OPEN = 295,        /* SQR_BRACKET_OPEN  */
    SQR_BRACKET_CLOSE = 296        /* SQR_BRACKET_CLOSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "parser.y"

    struct token * token;
    struct binary_operator * binary_operator;
    struct unary_operator * unary_operator;
    struct expression * expression;

#line 112 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
