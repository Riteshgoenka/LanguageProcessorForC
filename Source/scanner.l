%{
#include "parser.tab.h"	
#include <cstdlib>
%}

DIGIT [0-9]
ARITHOP [-+*/]
ASSIGNOP [=]
METACHAR [,;(){}?:]
LETTERS [a-zA-Z_]
NEXTLETTERS [a-zA-Z_0-9]
NAME {LETTERS}{NEXTLETTERS}*
NUM {DIGIT}+
FNUM ({DIGIT}*"."{DIGIT}+|{DIGIT}+"."{DIGIT}*)
INTEGER "int"
FLOAT "float"
VOID "void"
RETURN "return"
WHITESPACE [ \t]
NEWLINE [\n]
COMMENT "//".*
DOUBLESEMICOLONCOMMENT ";;".*
WHILE "while"
DO "do"
IF "if"
ELSE "else"
LT "<"
LTE "<="
GT ">"
GTE ">="
E "=="
NE "!="
AND "&&"
OR "||"
NOT "!"
PRINT "print"

%%

{COMMENT} 	;

{DOUBLESEMICOLONCOMMENT}	;

{RETURN}		{
				store_token_name("RETURN", yytext, yylineno);
				return RETURN;
			}

{PRINT}		{
				store_token_name("PRINT", yytext, yylineno);
				return PRINT;
			}

{WHILE}		{
				store_token_name("WHILE", yytext, yylineno);
				return WHILE;
			}

{DO}		{
				store_token_name("DO", yytext, yylineno);
				return DO;
			}

{IF}		{
				store_token_name("IF", yytext, yylineno);
				return IF;
			}

{ELSE}		{
				store_token_name("ELSE", yytext, yylineno);
				return ELSE;
			}

{LT} 		{
				store_token_name("LESS_THAN", yytext, yylineno);
				return LT;
			}

{LTE} 		{
				store_token_name("LESS_THAN_EQUAL", yytext, yylineno);
				return LTE;
			}

{GT} 		{
				store_token_name("GREATER_THAN", yytext, yylineno);
				return GT;
			}

{GTE} 		{
				store_token_name("GREATER_THAN_EQUAL", yytext, yylineno);
				return GTE;
			}

{E} 		{
				store_token_name("EQUAL", yytext, yylineno);
				return E;
			}

{NE} 		{
				store_token_name("NOT_EQUAL", yytext, yylineno);
				return NE;
			}

{AND} 		{
				store_token_name("AND", yytext, yylineno);
				return AND;
			}

{OR} 		{
				store_token_name("OR", yytext, yylineno);
				return OR;
			}

{NOT} 		{
				store_token_name("NOT", yytext, yylineno);
				return NOT;
			}

{NUM} 		{ 
				store_token_name("NUM", yytext, yylineno);
				yylval.integer_value = atoi(yytext);
				return INTEGER_NUMBER;
			}

{FNUM} 		{
				store_token_name("FNUM", yytext, yylineno);
				yylval.double_value = atof(yytext);
				return DOUBLE_NUMBER;
			}

{INTEGER} 	{
				store_token_name("INTEGER", yytext, yylineno);
				return INTEGER;
			}

{FLOAT} 	{
				store_token_name("FLOAT", yytext, yylineno);
				return FLOAT;
			}

{VOID} 		{
				store_token_name("VOID", yytext, yylineno);
				return VOID;
			}			

{NAME} 		{ 
				store_token_name("NAME", yytext, yylineno);
				std::string* s = new std::string(yytext);
				yylval.string_value = s;
				return NAME;
			}

{ARITHOP} 	{
				store_token_name("ARITHOP", yytext, yylineno);
				return yytext[0];
			}

{ASSIGNOP} 	{ 
				store_token_name("ASSIGN_OP", yytext, yylineno);
				return ASSIGN;
			}

{METACHAR} 	{
				store_token_name("META CHAR", yytext, yylineno);
				return yytext[0];
			}

{NEWLINE} 	{
				//yylineno++;
			}

{WHITESPACE} ;




 . 			{
                /* The difference with the previous submission is that
                    ERROR is changed to Error, and it is printed on stdout as well */
				fprintf(stderr, "cs316: Error in Lexer: %s\n", yytext);
				fprintf(stdout, "cs316: Error in Lexer: %s\n", yytext);
			}
