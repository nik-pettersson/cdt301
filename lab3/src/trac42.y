/* $Id: trac42.y 88 2007-12-19 10:14:01Z csg $
   FILE:       trac42.y
   CONTENTS:   trac42.y - This is the syntax analysator for the TRAC42-language
   REVISION:   1.0, 960917, Christer Eriksson & Jukka Maki-Turja
               1.1, 000824, Christer Sandberg Changes:bool-const now of type yyInt to make
                            the value from lex available.
   DESCRIPTION: Compiler skeleton used in a compiler theory course. To be translated by Bison
                and compiled to a syntax checker. Contains the basics for syntax checking. To
                be extended with semantic rules for building an AST, and calls to name analysis,
                type analysis, offset calculation and code generation. Plug in trac42.l as scanner.
*/

%{
#include <stdio.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>
#include "ast.h"        /* Data structures and constructors for the AST */
#include "trac42.h"     /* Various definitions */
#include "symbol.h"
#include "emit.h"
#include "trac42.y.h"   /* Declaration of tokens, genereated by bison. */

extern FILE *yyin;
int yyerror(const char *s);
int yylex(void);

%}
/* Alternative types of teh elements on the parse-value stack */
%union {
   t_tree        yyNode;
   intStruct    yyInt;
   stringStruct yyString;
   typeStruct   yyType;
   opStruct     yyOperator;
   int          yyLineNr;
};

/* Specifies operator precedence and what's on parse stack for these operators */
%left  <yyOperator> OROP
%left  <yyOperator> ANDOP
%left  <yyOperator> RELOP
%left  <yyOperator> MINUSOP PLUSOP
%left  <yyOperator> MULOP
%right <yyOperator> NOTOP UNOP

/* Rule types on parse stack */
%type  <yyNode>   program functions function formals_non_emtpty formals formal
%type  <yyNode>   decls decl stmnts stmnt expr actuals exprs idents ident

/* Specifies the types of other tokens than the operators, when on the parse stack */
%token <yyType>   BASIC_TYPE
%token <yyString> ID STRING_CONST
%token <yyInt>    INT_CONST BOOL_CONST
%token <yyLineNr> IF ELSE WHILE RETURN READ WRITE

%expect 1

%start program

%%

program     : functions
            ;

functions   : functions function
            | function
            ;

function    : BASIC_TYPE ID '(' formals ')' '{' decls stmnts '}' {$$ = NULL;}
            ;

formals     : formals_non_emtpty
            | {$$ = NULL;}
            ;

formals_non_emtpty : formals_non_emtpty ',' formal
            | formal
            ;

formal      : BASIC_TYPE ID {$$ = NULL;}
            ;

decls       : decls decl
            | {$$ = NULL;}
            ;

decl        : BASIC_TYPE idents ';'   {$$ = NULL;}
            ;

idents      : idents ',' ident
            | ident
            ;

ident       : ID                      {$$ = NULL;}
            ;

stmnts      : stmnts stmnt
            | stmnt
            ;

stmnt       : ID '=' expr ';'                       {$$ = NULL;}
            | IF '(' expr ')' stmnt ELSE stmnt      {$$ = NULL;}
            | IF '(' expr ')' stmnt                 {$$ = NULL;}
            | WHILE '(' expr ')' stmnt              {$$ = NULL;}
            | RETURN expr ';'                       {$$ = NULL;}
            | READ ID ';'                           {$$ = NULL;}
            | WRITE expr ';'                        {$$ = NULL;}
            | '{' stmnts '}'                        {$$ = NULL;}
            | ID '(' actuals ')' ';'                {$$ = NULL;}
            ;

expr        : MINUSOP expr %prec UNOP {$$ = NULL;}
            | NOTOP expr              {$$ = NULL;}
            | expr PLUSOP expr
            | expr MINUSOP expr
            | expr MULOP expr
            | expr ANDOP expr
            | expr OROP expr
            | expr RELOP expr
            |'(' expr ')'             {$$ = NULL;}
            | ID '(' actuals ')'      {$$ = NULL;}
            | ID                      {$$ = NULL;}
            | INT_CONST               {$$ = NULL;}
            | BOOL_CONST              {$$ = NULL;}
            | STRING_CONST            {$$ = NULL;}
            ;

actuals     : exprs
            | {$$ = NULL;}
            ;

exprs       : exprs ',' expr
            | expr
            ;

%%
int main (int argc, char *argv[])
{
   int syntax_errors;
   char *p, *objname, *basename, *lstname;

   if (argc <= 1) {
      fprintf (stderr, "No source file specified.\n");
   } else {
      yyin = fopen (argv[1], "r+");
      if (yyin == NULL) {
         fprintf (stderr, "Could not open input file %s\n", argv[1]);
      } else {
         basename = malloc(strlen(argv[1]) + 5);
         strcpy(basename, argv[1]);
         p = strstr (basename, ".t42");
         if (p) {
            *p = 0;
         }
         objname = malloc(strlen(basename) + 5);
         lstname = malloc(strlen(basename) + 5);
         strcpy(objname, basename);
         strcpy(lstname, basename);
         strcat(objname, ".obj");
         strcat(lstname, ".lst");

         syntax_errors = yyparse();
         if (!syntax_errors) {
            fprintf (stderr, "The answer is 42\n");
         } else {
            fprintf (stderr, "There were syntax errors.\n");
         }
         free(basename);
         free(objname);
         free(lstname);
      }
   }
   return 0;
}

int yyerror(const char *s)
{
   extern int yylineno;
   fprintf (stderr, "A %s at line %d\n",s, yylineno);
   return 0;
}

