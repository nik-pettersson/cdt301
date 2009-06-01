#include "printerror.h"

int yyerror(const char *s)
{
   extern int yylineno;
   fprintf (stderr, "%s at line %d\n",s, yylineno);
   return 0;
}
