%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdbool.h>

	void yyerror();
	extern char *yytext;
	extern int yylex();
	extern FILE *yyin;
	bool need_nl(void);
	static bool need_nl_flag = false;
	
	void Codeprnt(char * arg);
%}
%union
{
	int num;
	char *id;
}	



%token PROGRAMM
%token SEM
%token VAR
%token <num> INTT
%token <id> CHARR
%token FUN
%token ENDFUN
%token IFF
%token ENDIFF
%token ELSEIFF
%token THENN
%token ELSEE
%token SWITCHH
%token ENDSWITCHH
%token CASEE
%token DEFAULL
%token WHILEE
%token ENDWHILEE
%token FORR
%token ENDFORR
%token TTO
%token STEPP
%token STARTTMAIN
%token ENDDMAIN
%token PRINTT
%token RETURNNN
%token BREAKK
%token <id> STRUCTT
%token <id> ENDSTRUCTT
%token <id> TYPEDEFF
%token FORREQUALL
%token COMMENTT
%token ASS
%token PANKAT
%token COMMA
%token LPAR
%token RPAR
%token LBR
%token RBR
%token ANDD
%token ORR
%token PLUS
%token MINUS
%token POWERR
%token MULT
%token DIVV
%token EQUAL
%token DIFF
%token SM
%token BIG
%token ID
%token <int> NUM

%type <id> ID
%type EKFRASH
%type VALUE

%type <id> STRUCTDEFTWO
%start PROGRAM


%% 

/* Program directive. */
PROGDECL : PROGRAMM { need_nl_flag = true; } ID '\n';

/* Function definition */
FUNDEF : FUN ID LPAR PARAMETERS { need_nl_flag = true; } RPAR'\n';

/*grammar rules*/
PROGRAM : PROGDECL PROGRAMBODY ;
PROGRAMBODY : STRUCT FUNCTION MAIN;

STRUCT : STRUCTDEF STRUCTBODY | STRUCTDEFTWO | ;
STRUCTDEF : STRUCTT { need_nl_flag = true; } ID'\n';
STRUCTBODY : VARDECLERATION ENDSTRUCTT;
STRUCTDEFTWO : TYPEDEFF STRUCTT { need_nl_flag = true; } ID '\n' ID ENDSTRUCTT
{
	if(strcmp($4, $6) != 0 )
	{
		printf("Struct definition different names %s and %s \n",  $4, $6);
		exit(0);
	}
};

FUNCTION : FUNDEF FUNCTIONBLOCK | ;
PARAMETERS : VARS A | ;
A : COMMA VARS A | ;
FUNCTIONBLOCK : VARDECLERATION BLOCKCODE RETURNNN RETURNN ENDFUN ;

MAIN : MAINHEADER MAINBODY ;
MAINHEADER : STARTTMAIN ;
MAINBODY : VARDECLERATION BLOCKCODE ENDDMAIN ;


VARDECLERATION : VAR VARDEC  | ;

VARDEC : VARS ARRAY M VARDEC | ;

ARRAY : LBR NUM RBR | ;
VARS : DATATYPE ID ;
DATATYPE : INTT | CHARR ;
M :  COMMA  ID ARRAY M | SEM ;
RETURNN : ID | VARS | NUM ;

BLOCKCODE : CODE BLOCKCODE | ;
CODE :  ANATHESHS | BROXOU | ELEGXOU | EKTUPOSH | TERMATISMOU | SXOLIA ;
ANATHESHS : ID ASS EKFRASH SEM ;
EKFRASH : VALUE ARITHMETICEXP EKFRASH | VALUE ;

ARITHMETICEXP : PLUS | MINUS | POWERR | MULT | DIVV ;


BROXOU : WHILEE CONDITION BLOCKCODE ENDWHILEE | FORR ID FORREQUALL NUM TTO NUM STEPP NUM BLOCKCODE ENDFORR ;
CONDITION : VALUE OPERATOR CONDITION | VALUE ;


OPERATOR : SM | BIG | EQUAL | DIFF | ANDD | ORR ;

ELEGXOU : IF | SWITCH ;

IF : IFF CONDITION THENN BLOCKCODE ELSEIF ELSE ENDIFF ;
SWITCH : SWITCHH CONDITION CASE DEFAULTCASE ENDSWITCHH ;

ELSEIF : ELSEIFF EKFRASH THENN BLOCKCODE ELSEIF | ;
ELSE : ELSEE BLOCKCODE | ;
CASE : CASEE EKFRASH PANKAT BLOCKCODE CASE | CASEE EKFRASH PANKAT BLOCKCODE ;
DEFAULTCASE : DEFAULL PANKAT BLOCKCODE | ;

EKTUPOSH : PRINTT LPAR  VALUE LBR COMMA VARS RBR RPAR SEM ;

TERMATISMOU : BREAKK SEM ;

SXOLIA : COMMENTT { need_nl_flag = true; } ID '\n';

VALUE : ID | NUM ;


%%




bool need_nl(void) {
  bool temp = need_nl_flag;
  need_nl_flag = false;
  return temp;
}



int main(int argc, char **argv){
	
	//#ifdef YYDEBUG
  	//yydebug = 1;
	//#endif
	int flag;
	//elegxoume an exoume 2o orisma, dld an dothike onoma arxeiou c
	if(argc==2)
	{
		yyin = fopen( argv[1], "r" );
	}
	else
	{
		yyin = stdin;
	}
	flag = yyparse();
	
	Codeprnt(argv[1]);
	printf("No syntax error. \n");
	return flag;
}



void Codeprnt(char * arg)
{
    char c;
    FILE *fptr;
    fptr = fopen(arg, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
  
    fclose(fptr);
}

