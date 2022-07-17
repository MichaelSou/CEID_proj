%option noyywrap
%option yylineno



%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ask.tab.h"
#include <stdbool.h>





bool need_nl(void);
void yyerror();

%}


alpha       [a-zA-Z]
digit       [0-9]
alnum       {alpha}|{digit}

ID          {alpha}+{alnum}*
INTEGER (int)
CHAR (char)

%x ML_COMMENT

%%

"PROGRAM" {return PROGRAMM;}
";" {return SEM;}
"VARS" {return VAR;}
"CHAR" {return CHARR;}
"INTEGER" {return INTT;}
"FUNCTION" {return FUN;}
"END_FUNCTION" {return ENDFUN;}
"IF" {return ENDIFF;}
"ENDIF" {return IFF;}
"ELSEIF" {return ELSEIFF;}
"ELSE" {return ELSEE;}
"THEN" {return THENN;}
"SWITCH" {return SWITCHH;}
"ENDSWITCH" {return ENDSWITCHH;}
"CASE" {return CASEE;}
"DEFAULT" {return DEFAULL;}
"WHILE" {return WHILEE;}
"ENDWHILE" {return ENDWHILEE;}
"FOR" {return FORR;}
"ENDFOR" {return ENDFORR;}
"TO" {return TTO;}
"STEP" {return STEPP;}
"STARTMAIN" {return STARTTMAIN;}
"ENDMAIN" {return ENDDMAIN;}
"RETURN" {return RETURNNN;}
"PRINT" {return PRINTT;}
"BREAK" {return BREAKK;}
"STRUCT" {return STRUCTT;}
"ENDSTRUCT" {return ENDSTRUCTT;}
"TYPEDEF" {return TYPEDEFF;}
":=" {return FORREQUALL;}
"%" {return COMMENTT;}
"=" {return ASS;}
":" {return PANKAT;}
"," {return COMMA;}
"(" {return LPAR;}
")" {return RPAR;}
"[" {return LBR;}
"]" {return RBR;}
"AND" {return ANDD;}
"OR" {return ORR;}
"+" {return PLUS;}
"-" {return MINUS;}
"^" {return POWERR;}
"*" {return MULT;}
"/" {return DIVV;}
"==" {return EQUAL;}
"!=" {return DIFF;}
"<" {return SM;}
">" {return BIG;}
[0-9]+  {	
		yylval.num = atoi(yytext); 
		return NUM;
	}
([[:space:]]{-}[\n])+  { }
{ID} 	{
		yylval.id = strdup(yytext); 
		return ID;
	}

<INITIAL>"/*"                   BEGIN(ML_COMMENT);
<ML_COMMENT>"*/"                BEGIN(INITIAL);  
<ML_COMMENT>. | "\n"            { }

[[:space:]]+           { if (need_nl()) return '\n';}
.               { yyerror("Unrecognized character"); }
%%

void yyerror()          
{
  fprintf(stderr, "Syntax error at line %d\n", yylineno);
  exit(1);
}


