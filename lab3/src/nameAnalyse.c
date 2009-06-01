#include "nameAnalyse.h"

t_symtab *globalSymbols;
t_symtab *current;

int nameType(t_tree n){
	int error = 0;
	if(n != NULL) {
		switch(n->Kind){
			case kProgram:
				nameType(n->Node.Program.Functions);
				break;
			case kFunction:
				if(symtab_exist(globalSymbols, n->Node.Function.Name)) {
					error++;
					yyerror("Multiple declarations of function");
				}
				else {
					symtab_add(globalSymbols, n->Node.Function.Name);
					printf("Added %s to global symbols\n", n->Node.Function.Name);
				}
				error += nameType(n->Node.Function.Next);
				n->Node.Function.Symbols = symtab_create();
				current = NULL;
				current = n->Node.Function.Symbols;
				//check here
				printf("All clear\n");
				error += nameType(n->Node.Function.Variables);
				error += nameType(n->Node.Function.Stmnts);
				printf("Done \n");
				break;
			case kVariable:
				/* Do name analysing here */
				if(symtab_exist(current,n->Node.Variable.Name)) {
					error++;
					yyerror("Multiple declarations of variable");
				}
				else {
					symtab_add(current, n->Node.Variable.Name);
					printf("Added %s to local symbols\n", n->Node.Variable.Name);
				}
				error += nameType(n->Node.Variable.Next);
				break;
			case kAssign:
				//check here
				printf("assign\n");
				if(!symtab_exist(current, n->Node.Assign.Id)) {
					error++;
					yyerror("Unknown symbol");
				}
				error += nameType(n->Node.Assign.Expr);
				error += nameType(n->Node.Assign.Next);
				break;
			case kIf:
				error += nameType(n->Node.If.Expr);
				error += nameType(n->Node.If.Then);
				error += nameType(n->Node.If.Else);
				error += nameType(n->Node.If.Next);
				break;
			case kWhile:
				error += nameType(n->Node.While.Expr);
				error += nameType(n->Node.While.Stmnt);
				error += nameType(n->Node.While.Next);
				break;
			case kRead:
				//check here
				printf("read\n");
				if(!symtab_exist(current, n->Node.Read.Id)) {
					error++;
					yyerror("Unknown symbol");
				}
				error += nameType(n->Node.Read.Next);
				break;
			case kWrite:
				printf("write\n");
				error += nameType(n->Node.Write.Expr);
				error += nameType(n->Node.Write.Next);
				break;
			case kFuncCallStmnt:
				printf("FuncCallStmnt\n");
				//check here
				if(!symtab_exist(globalSymbols, n->Node.FuncCallStmnt.FuncName)) {
					error++;
					yyerror("Unknown symbol");
				}
				error += nameType(n->Node.FuncCallStmnt.Actuals);
				error += nameType(n->Node.FuncCallStmnt.Next);
				break;
			case kReturn:
				error += nameType(n->Node.Return.Expr);
				error += nameType(n->Node.Return.Next);
				break;
			case kActual:
				printf("actuals\n");
				error += nameType(n->Node.Actual.Expr);
				error += nameType(n->Node.Actual.Next);
				break;
			case kUnary:
				error += nameType(n->Node.Unary.Expr);
				break;
			case kBinary:
				error += nameType(n->Node.Binary.LeftOperand);
				error += nameType(n->Node.Binary.RightOperand);
				break;
			case kIntConst:
			case kBoolConst:
			case kStringConst:
				break;
			case kFuncCallExpr:
				printf("FuncCallExpr\n");
				//check here
				if(!symtab_exist(globalSymbols, n->Node.FuncCallExpr.FuncName)) {
					error++;
					yyerror("Unknown symbol");
				}
				error += nameType(n->Node.FuncCallExpr.Actuals);
				printf("FuncCallExpr done\n");
				break;
			case kRValue:
				//check here
				if(!symtab_exist(current, n->Node.RValue.Id)) {
					error++;
					yyerror("Unknown symbol");
				}
				break;
		}
	}
	return error;
}

int nameAnalyse(t_tree root){
	//error = 0;
	return nameType(root);
	/*if(error != 0){
		fprintf(stderr,"\nName analyse failed %d times\n", error);
		exit(-1);
	}
	else
		printf("\nName analyse completed\n");*/
}

