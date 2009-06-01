#include "printast.h"

void printAST(t_tree n) {
	if(n != NULL){
		switch(n->Kind){
			case kProgram:
				printAST(n->Node.Program.Functions);
				break;
			case kFunction:
				printf("Function: %s\n", n->Node.Function.Name);
				printf("\tVariables:\n");
				printAST(n->Node.Function.Variables);
				printf("\tStatements:\n");
				printAST(n->Node.Function.Stmnts);
				printf("End %s\n\n", n->Node.Function.Name);
				printAST(n->Node.Function.Next);
				break;
			case kVariable:
				printf("\t%d: %s %s\n", n->Node.Variable.VarKind, varType(n->Node.Variable.Type), n->Node.Variable.Name);
				printAST(n->Node.Variable.Next);
				break;
			case kAssign:
				printf("\t%s := ", n->Node.Assign.Id);
				printAST(n->Node.Assign.Expr);
				printf("\n");
				printAST(n->Node.Assign.Next);
				break;
			case kIf:
				printf("\tIf( ");
				printAST(n->Node.If.Expr);
				printf(" ) Then:\n");
				printAST(n->Node.If.Then);
				if(n->Node.If.Else != NULL) {
					printf("\n\tElse:\n");
					printAST(n->Node.If.Else);
				}
				printf("\tEnd If\n");
				printAST(n->Node.If.Next);
				break;
			case kWhile:
				printf("\tWhile( ");
				printAST(n->Node.While.Expr);
				printf(" ) \n");
				printAST(n->Node.While.Stmnt);
				printf("\tEnd While\n");
				printAST(n->Node.While.Next);
				break;
			case kRead:
				printf("\tRead: %s\n", n->Node.Read.Id);
				printAST(n->Node.Read.Next);
				break;
			case kWrite:
				printf("\tWrite: ");
				printAST(n->Node.Write.Expr);
				printf("\n");
				printAST(n->Node.Write.Next);
				break;
			case kFuncCallStmnt:
				printf("\t%s: ", n->Node.FuncCallStmnt.FuncName);
				printAST(n->Node.FuncCallStmnt.Actuals);
				printf("\n");
				printAST(n->Node.FuncCallStmnt.Next);
				break;
			case kReturn:
				printf("\tReturn: ");
				printAST(n->Node.Return.Expr);
				printf("\n");
				printAST(n->Node.Return.Next);
				break;
			case kActual:
				printAST(n->Node.Actual.Expr);
				if(n->Node.Actual.Next != NULL)
					printf(", ");
				printAST(n->Node.Actual.Next);
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
				printAST(n->Node.Unary.Expr);
				printf("\n");
				break;
			case kBinary:
				printAST(n->Node.Binary.LeftOperand);
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
				printAST(n->Node.Binary.RightOperand);
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
				printAST(n->Node.FuncCallExpr.Actuals);
				printf(" ");
				break;
			case kRValue:
				printf("%s", n->Node.RValue.Id);
				break;
		}
	}
}

