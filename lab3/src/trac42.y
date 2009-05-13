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
#include <unistd.h>
#include <stdlib.h>
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

t_symtab *globalSymbols;
t_symtab * current;
int error;

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

program     : functions {treeRoot = mProgram($1); printf("mProgram\n");}
            ;

functions   : functions function {$$ = connectFunctions($1, $2); printf("connectFunctions\n");}
            | function {$$ = $1; printf("\n");} 
            ;

function    : BASIC_TYPE ID '(' formals ')' '{' decls stmnts '}' {$$ = mFunction(connectVariables($4, $7), $8, $2.strVal, $1.type, $2.lineNr); printf("%d %s at %d\n", $1.type, $2.strVal, $2.lineNr);
							}
            ;

formals     : formals_non_emtpty {$$ = $1;}
            | {$$ = NULL;}
            ;

formals_non_emtpty : formals_non_emtpty ',' formal {$$ = connectVariables($1, $3);}
            | formal {$$ = $1;}
            ;

formal      : BASIC_TYPE ID {$$ = mVariable(kFormal, $2.strVal, $1.type, $2.lineNr); printf("%d %s\n", $1.type, $2.strVal);}
            ;

decls       : decls decl {$$ = connectVariables($1, $2);}
            | {$$ = NULL;}
            ;

decl        : BASIC_TYPE idents ';'   {$$ = addType($2, $1.type); }
            ;

idents      : idents ',' ident {$$ = connectVariables($1, $3);}
            | ident {$$ = $1;}
            ;

ident       : ID                      {$$ = mVariable(kLocal, $1.strVal, VOID, $1.lineNr); }
            ;

stmnts      : stmnts stmnt { $$ = connectStmnts($1, $2);}
            | stmnt {$$ = $1;}
            ;

stmnt       : ID '=' expr ';'                       {$$ = mAssign($1.strVal, $3, $1.lineNr);}
            | IF '(' expr ')' stmnt ELSE stmnt      {$$ = mIf($3, $5, $7, $1);}
            | IF '(' expr ')' stmnt                 {$$ = mIf($3, $5, NULL, $1);}
            | WHILE '(' expr ')' stmnt              {$$ = mWhile($3, $5, $1);}
            | RETURN expr ';'                       {$$ = mReturn($2, $1);}
            | READ ID ';'                           {$$ = mRead($2.strVal, $1);}
            | WRITE expr ';'                        {$$ = mWrite($2, $1);}
            | '{' stmnts '}'                        {$$ = $2;}
            | ID '(' actuals ')' ';'                {$$ = mFuncCallStmnt($3, $1.strVal, $1.lineNr);}
            ;

expr        : MINUSOP expr %prec UNOP {$$ = mUnary(NEG, $2, $1.lineNr);}
            | NOTOP expr              {$$ = mUnary(NOT, $2, $1.lineNr);}
            | expr PLUSOP expr				{$$ = mBinary($1, $2.opType, $3, $2.lineNr);}
            | expr MINUSOP expr				{$$ = mBinary($1, $2.opType, $3, $2.lineNr);}
            | expr MULOP expr					{$$ = mBinary($1, $2.opType, $3, $2.lineNr);}
            | expr ANDOP expr					{$$ = mBinary($1, $2.opType, $3, $2.lineNr);}
            | expr OROP expr					{$$ = mBinary($1, $2.opType, $3, $2.lineNr);}
            | expr RELOP expr					{$$ = mBinary($1, $2.opType, $3, $2.lineNr);}
            |'(' expr ')'             {$$ = $2;}
            | ID '(' actuals ')'      {$$ = mFuncCallExpr($3, $1.strVal, $1.lineNr);}
            | ID                      {$$ = mRValue($1.strVal, $1.lineNr);}
            | INT_CONST               {$$ = mIntConst($1.intVal, $1.lineNr);}
            | BOOL_CONST              {$$ = mBoolConst($1.intVal, $1.lineNr);}
            | STRING_CONST            {$$ = mStringConst($1.strVal, $1.lineNr);}
            ;

actuals     : exprs {$$ = $1;}
            | {$$ = NULL;}
            ;

exprs       : exprs ',' expr {$$ = connectActuals($1, mActual($3));}
            | expr	{$$ = mActual($1);}
            ;

%%

char *varType(eType t) {
	switch (t) {
		case VOID:
			return "void";
		case BOOL:
			return "bool";
		case INT:
			return "int";
		case STRING:
			return "string";
	}
	return "unknown";
}

void nodeType(t_tree n){
	if(n != NULL){
		switch(n->Kind){
			case kProgram:
				nodeType(n->Node.Program.Functions);
				break;
			case kFunction:
				printf("Function: %s\n", n->Node.Function.Name);
				printf("\tVariables:\n");
				nodeType(n->Node.Function.Variables);
				printf("\tStatements:\n");
				nodeType(n->Node.Function.Stmnts);
				printf("End %s\n\n", n->Node.Function.Name);
				nodeType(n->Node.Function.Next);
				break;
			case kVariable:
				printf("\t%d: %s %s\n", n->Node.Variable.VarKind, varType(n->Node.Variable.Type), n->Node.Variable.Name);
				nodeType(n->Node.Variable.Next);
				break;
			case kAssign:
				printf("\t%s := ", n->Node.Assign.Id);
				nodeType(n->Node.Assign.Expr);
				printf("\n");
				nodeType(n->Node.Assign.Next);
				break;
			case kIf:
				printf("\tIf( ");
				nodeType(n->Node.If.Expr);
				printf(" ) Then:\n");
				nodeType(n->Node.If.Then);
				if(n->Node.If.Else != NULL) {
					printf("\n\tElse:\n");
					nodeType(n->Node.If.Else);
				}
				printf("\tEnd If\n");
				nodeType(n->Node.If.Next);
				break;
			case kWhile:
				printf("\tWhile( ");
				nodeType(n->Node.While.Expr);
				printf(" ) \n");
				nodeType(n->Node.While.Stmnt);
				printf("\tEnd While\n");
				nodeType(n->Node.While.Next);
				break;
			case kRead:
				printf("\tRead: %s\n", n->Node.Read.Id);
				nodeType(n->Node.Read.Next);
				break;
			case kWrite:
				printf("\tWrite: ");
				nodeType(n->Node.Write.Expr);
				printf("\n");
				nodeType(n->Node.Write.Next);
				break;
			case kFuncCallStmnt:
				printf("\t%s: ", n->Node.FuncCallStmnt.FuncName);
				nodeType(n->Node.FuncCallStmnt.Actuals);
				printf("\n");
				nodeType(n->Node.FuncCallStmnt.Next);
				break;
			case kReturn:
				printf("\tReturn: ");
				nodeType(n->Node.Return.Expr);
				printf("\n");
				nodeType(n->Node.Return.Next);
				break;
			case kActual:
				nodeType(n->Node.Actual.Expr);
				if(n->Node.Actual.Next != NULL)
					printf(", ");
				nodeType(n->Node.Actual.Next);
				break;
			case kUnary:
				switch(n->Node.Unary.Operator){
					case NOT:
						printf("!");
						break;
					case NEG:
						printf("-");
						break;
				}
				nodeType(n->Node.Unary.Expr);
				printf("\n");
				break;
			case kBinary:
				nodeType(n->Node.Binary.LeftOperand);
				switch(n->Node.Binary.Operator){
					case SUB:
						printf(" - ");
						break;
					case PLUS:
						printf(" + ");
						break;
					case MULT:
						printf(" * ");
						break;
					case DIV:
						printf(" / ");
						break;
					case OR:
						printf(" or ");
						break;
					case AND:
						printf(" and ");
						break;
					case EQ:
						printf(" == ");
						break;
					case LT:
						printf(" < ");
						break;
					case LE:
						printf(" <= ");
						break;
				}
				nodeType(n->Node.Binary.RightOperand);
				break;
			case kIntConst:
				printf("%d", n->Node.IntConst.Value);
				break;
			case kBoolConst:
				printf("%d", n->Node.BoolConst.Value);
				break;
			case kStringConst:
				printf("%s", n->Node.StringConst.Value);
				break;
			case kFuncCallExpr:
				printf("%s:", n->Node.FuncCallExpr.FuncName);
				nodeType(n->Node.FuncCallExpr.Actuals);
				printf(" ");
				break;
			case kRValue:
				printf("%s", n->Node.RValue.Id);
				break;
		}
	}
}

void printAST(t_tree root){
	nodeType(root);
}

/**
	Name analysing
 */
void nameType(t_tree n){
	if(n != NULL){
		switch(n->Kind){
			case kProgram:
				nameType(n->Node.Program.Functions);
				break;
			case kFunction:
				if(symtab_exist(globalSymbols, n->Node.Function.Name))
					error++;
				else
					symtab_add(globalSymbols, n->Node.Function.Name);
				nameType(n->Node.Function.Next);
				n->Node.Function.Symbols = symtab_create();
				current = NULL;
				current = n->Node.Function.Symbols;
				//check here
				printf("All clear\n");
				nameType(n->Node.Function.Variables);
				nameType(n->Node.Function.Stmnts);
				printf("Done \n");
				break;
			case kVariable:
				/* Do name analysing here */
				if(symtab_exist(current,n->Node.Variable.Name))
					error++;
				else
					symtab_add(current, n->Node.Variable.Name);
				nameType(n->Node.Variable.Next);
				break;
			case kAssign:
				//check here
				printf("assign\n");
				if(!symtab_exist(current, n->Node.Assign.Id))
					error++;
				nameType(n->Node.Assign.Expr);
				nameType(n->Node.Assign.Next);
				break;
			case kIf:
				nameType(n->Node.If.Expr);
				nameType(n->Node.If.Then);
				nameType(n->Node.If.Else);
				nameType(n->Node.If.Next);
				break;
			case kWhile:
				nameType(n->Node.While.Expr);
				nameType(n->Node.While.Stmnt);
				nameType(n->Node.While.Next);
				break;
			case kRead:
				//check here
				printf("read\n");
				if(!symtab_exist(current, n->Node.Read.Id))
					error++;
				nameType(n->Node.Read.Next);
				break;
			case kWrite:
				printf("write\n");
				nameType(n->Node.Write.Expr);
				nameType(n->Node.Write.Next);
				break;
			case kFuncCallStmnt:
				printf("FuncCallStmnt\n");
				//check here
				if(!symtab_exist(globalSymbols, n->Node.FuncCallStmnt.FuncName))
					error++;
				nameType(n->Node.FuncCallStmnt.Actuals);
				nameType(n->Node.FuncCallStmnt.Next);
				break;
			case kReturn:
				nameType(n->Node.Return.Expr);
				nameType(n->Node.Return.Next);
				break;
			case kActual:
				printf("actuals\n");
				nameType(n->Node.Actual.Expr);
				nameType(n->Node.Actual.Next);
				break;
			case kUnary:
				nameType(n->Node.Unary.Expr);
				break;
			case kBinary:
				nameType(n->Node.Binary.LeftOperand);
				nameType(n->Node.Binary.RightOperand);
				break;
			case kIntConst:
			case kBoolConst:
			case kStringConst:
				break;
			case kFuncCallExpr:
				printf("FuncCallExpr\n");
				//check here
				if(!symtab_exist(globalSymbols, n->Node.FuncCallExpr.FuncName))
					error++;
				nameType(n->Node.FuncCallExpr.Actuals);
				printf("FuncCallExpr done\n");
				break;
			case kRValue:
				//check here
				if(!symtab_exist(current, n->Node.RValue.Id))
					error++;
				break;
		}
	}
}

void nameAnalyse(t_tree root){
	error = 0;
	nameType(root);
	if(error != 0){
		fprintf(stderr,"\nName analyse failed %d times\n", error);
		exit(-1);
	}
	else
		printf("\nName analyse completed\n");
}

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
				 /*
						Our special code
				 */
				 globalSymbols = symtab_create();
				 printAST(treeRoot);
				 nameAnalyse(treeRoot);
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

