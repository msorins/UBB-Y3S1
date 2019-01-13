%{
#include "myscanner.h"
%}

%%


[ \t\n]                          ;
"main"      {printf("MAIN "); return MAIN;};
[{};=+\-|]  return OPERATOR;
"int"       {printf("INT "); return INT;};
"char"      return CHAR;
"string"    return STRING;
"return"    return BACK;
"cin"       return CIN;

[a-zA-Z][a-zA-Z0-9]{0,254}        {printf("%s ", yytext); return IDENTIFIER;}
([-]?[1-9][0-9]*)|[0]             {return NR;}
\"\w*\"                           {return CONSTANT;}
\'\w?\'                           {return CONSTANT;}


.          return -1;

%%

int yywrap()
{
    return 1;
}