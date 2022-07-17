/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_ASK_TAB_H_INCLUDED
# define YY_YY_ASK_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAMM = 258,
    SEM = 259,
    VAR = 260,
    INTT = 261,
    CHARR = 262,
    FUN = 263,
    ENDFUN = 264,
    IFF = 265,
    ENDIFF = 266,
    ELSEIFF = 267,
    THENN = 268,
    ELSEE = 269,
    SWITCHH = 270,
    ENDSWITCHH = 271,
    CASEE = 272,
    DEFAULL = 273,
    WHILEE = 274,
    ENDWHILEE = 275,
    FORR = 276,
    ENDFORR = 277,
    TTO = 278,
    STEPP = 279,
    STARTTMAIN = 280,
    ENDDMAIN = 281,
    PRINTT = 282,
    RETURNNN = 283,
    BREAKK = 284,
    STRUCTT = 285,
    ENDSTRUCTT = 286,
    TYPEDEFF = 287,
    FORREQUALL = 288,
    COMMENTT = 289,
    ASS = 290,
    PANKAT = 291,
    COMMA = 292,
    LPAR = 293,
    RPAR = 294,
    LBR = 295,
    RBR = 296,
    ANDD = 297,
    ORR = 298,
    PLUS = 299,
    MINUS = 300,
    POWERR = 301,
    MULT = 302,
    DIVV = 303,
    EQUAL = 304,
    DIFF = 305,
    SM = 306,
    BIG = 307,
    ID = 308,
    NUM = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "ask.y"

	int num;
	char *id;

#line 117 "ask.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ASK_TAB_H_INCLUDED  */
