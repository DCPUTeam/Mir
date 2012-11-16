%{

/**

	File:           lexer.y

	Project:        DCPU-16 Toolchain
	Component:      Tool-MsgGen

	Authors:        James Rhodes

	Description:    Defines lexer for the message definition generator.

**/

#ifdef _WIN32
#pragma warning(disable:4005;disable:4013;disable:4018)
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bstring.h>
#include <simclist.h>
#include "parser.h"

%}

%option noyywrap

%x STRING

%%

message                         return KEYWORD_MESSAGE;
\{                              return BRACKET_OPEN;
\}                              return BRACKET_CLOSE;
[a-zA-Z]+[0-9a-zA-Z_\:]*        yylval.word = bfromcstr(yytext); return NAME;
\;                              return SEMICOLON;
[ \t\r\n]+                      /* ignore whitespace */;

    /* Default Error Rule */

<*>.|\n                                 {
                                            char* out = malloc(50);
                                            memset(out, '\0', 50);
                                            snprintf(out, 49, "unexpected characters in input '%s' (%i).", yytext, yytext[0]);
                                            yyerror(out);
                                            free(out);
                                            return ERROR;
                                        }
%%
