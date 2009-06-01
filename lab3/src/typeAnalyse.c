#include "typeAnalyse.h"

t_symtab *globalSymbols;
t_symtab *current;

eType typeType(t_tree n){
	static eType funcRet = VOID;
	t_tree tmp;
	eType type;
	char str[4096];
	char * tmpStr;
	if(n != NULL){
		switch(n->Kind){
			case kProgram:
				printf("kProgram\n");
				return typeType(n->Node.Program.Functions);
				break;
			case kFunction:
				printf("kFunction\n");
				current = n->Node.Function.Symbols;
				funcRet =  n->Node.Function.Type;
				if(typeType(n->Node.Function.Variables) == VOID){
					printf("happy\n");
					strcpy(str, varType(n->Node.Function.Type));
					strcat(str, " -> ");
					for(tmp = n->Node.Function.Variables; tmp != NULL; tmp = tmp->Node.Variable.Next){
						printf("Iterating formals for '%s'\n", n->Node.Function.Name);
						if(tmp->Node.Variable.VarKind == kFormal){
							strcat(str, varType((eType)symtab_get(current, tmp->Node.Variable.Name)));
							strcat(str, " x ");
						}
					}
					printf("Setting '%s' -> '%s'\n", n->Node.Function.Name, str);
					symtab_set(globalSymbols, n->Node.Function.Name, (void *)str);

					if(typeType(n->Node.Function.Stmnts) == VOID) {
						printf("kFunction '%s' OK\n", n->Node.Function.Name);
						if(typeType(n->Node.Function.Next) != VOID) {
							printf("kFunction '%s' ERROR (next function != VOID)\n", n->Node.Function.Name);
							return ERROR;
						}
						else
							return VOID;
					}
					else{
						printf("kFunction: Error\n");
						return ERROR;
					}
				}
				break;
			case kVariable:
				printf("kvariable\n");
				if(symtab_exist(current, n->Node.Variable.Name)){
					printf("Setting %s -> %s\n", n->Node.Variable.Name, varType(n->Node.Variable.Type));
					symtab_set(current, n->Node.Variable.Name ,(void *)n->Node.Variable.Type);
					if(typeType(n->Node.Variable.Next) == VOID) {
						printf("kVariable '%s' OK\n", n->Node.Variable.Name);
						return VOID;
					}
					else {
						printf("kVariable '%s' ERROR (next variable != VOID)\n", n->Node.Variable.Name);
						return ERROR;
					}
				}
				else {
					yyerror("Unknown variable");
					//printf("kVariable '%s' ERROR (not in symbol table)\n", n->Node.Variable.Name);
					return ERROR;
				}
				break;
			case kAssign:
				if((eType)symtab_get(current, n->Node.Assign.Id) == typeType(n->Node.Assign.Expr)) {
					if(typeType(n->Node.Assign.Next) == VOID) {
						return VOID;
					}
					else {
						return ERROR;
					}
				}
				else {
					yyerror("Type missmatch");
				}
				/*if((eType)symtab_get(current,n->Node.Assign.Id) == typeType(n->Node.Assign.Expr) && typeType(n->Node.Assign.Next) == VOID){
					printf("kssign %s\n", n->Node.Assign.Id);
					return VOID;
				}
				else{
					printf("kAssign: Error %s\n", n->Node.Assign.Id);
					return ERROR;
				}*/
				break;
			case kIf:				
				printf("kIf\n");
				if(typeType(n->Node.If.Expr) == BOOL) {
					if( typeType(n->Node.If.Then) == VOID &&
							typeType(n->Node.If.Else) == VOID &&
							typeType(n->Node.If.Next) == VOID) {
						return VOID;
					}
					else
						return ERROR;
				}
				else {
					yyerror("Expected type BOOL for expression");
					return ERROR;
				}
				/*if(typeType(n->Node.If.Expr) == BOOL && typeType(n->Node.If.Then) == VOID && typeType(n->Node.If.Else) == VOID && typeType(n->Node.If.Next) == VOID)
					return VOID;
				else{
					printf("kIf: Error\n");
					return ERROR;
				}*/
				break;
			case kWhile:
				printf("kWhile\n");
				if(typeType(n->Node.While.Expr) == BOOL) {
					if(typeType(n->Node.While.Stmnt) == VOID && typeType(n->Node.While.Next) == VOID)
						return VOID;
					else
						return ERROR;
				}
				else {
					yyerror("Expected type BOOL for expression");
					return ERROR;
				}
				/*if(typeType(n->Node.While.Expr) == BOOL && typeType(n->Node.While.Stmnt) == VOID && typeType(n->Node.While.Next) == VOID)
					return VOID;
				else{
					printf("kWhile: Error\n");
					return ERROR;
				}*/
				break;
			case kRead:
				printf("kRead %d %s\n", symtab_exist(current, n->Node.Read.Id), n->Node.Read.Id);
				if(symtab_exist(current, n->Node.Read.Id) && typeType(n->Node.Read.Next) == VOID)
					return VOID;
				else{
					printf("kRead error\n");
					return ERROR;
				}
				break;
			case kWrite:
				printf("kWrite\n");
				if(typeType(n->Node.Write.Expr) != ERROR && typeType(n->Node.Write.Next) == VOID)
					return VOID;
				else{
					printf("kWrite: Error next write and or Expr\n");
					return ERROR;
				}
				break;
			case kFuncCallStmnt:
				printf("kFuncCallStmnt\n");
				if(typeType(n->Node.FuncCallStmnt.Next) == VOID && typeType(n->Node.FuncCallStmnt.Actuals) == VOID){
					strcpy(str, "> ");
					for(tmp = n->Node.FuncCallStmnt.Actuals; tmp != NULL; tmp = tmp->Node.Actual.Next){
						type = typeType(tmp->Node.Actual.Expr);
						if(type != VOID) {
							strcat(str, varType(typeType(tmp->Node.Actual.Expr)));
							strcat(str, " x ");
						}
					}
					tmpStr = strchr((char *)symtab_get(globalSymbols, n->Node.FuncCallStmnt.FuncName),'>');
					printf("str: x%sx\n", str);
					printf("str: x%sx\n", tmpStr);
					printf("!str: %s\n" ,(char *)symtab_get(globalSymbols, n->Node.FuncCallStmnt.FuncName));
					if(strncmp(str, tmpStr,strlen(tmpStr)) == 0){
						printf("returning void\n");
						return VOID;
					}
					else {
						printf("strncmp(str, strchr(.........)\n"); 
						yyerror("Parameter missmatch for function call");
						return ERROR;
					}
				}
				else{
					printf("kFuncCallStmnt: Error \n");
					return ERROR;
				}
				break;
			case kReturn:
				printf("kReturn\n");
				if(typeType(n->Node.Return.Next) == VOID && typeType(n->Node.Return.Expr) != ERROR) {
					if(funcRet == typeType(n->Node.Return.Expr))
						return VOID;
					else{
						printf("kReturn: Error\n");
						yyerror("Return type for function invalid");
						return ERROR;
					}
				}
				else {
					printf("kReturn: Error\n");
					return ERROR;
				}
				break;
			case kActual:
				printf("kActual\n");
				if(typeType(n->Node.Actual.Next) == VOID && typeType(n->Node.Actual.Expr) != ERROR)
					return VOID;
				else{
					printf("kActual: error\n");
					return ERROR;
				}
				break;
			case kUnary:
				printf("kUnary\n");
				return typeType(n->Node.Unary.Expr);	
				break;
			case kBinary:
				printf("kbinary\n");
				if(typeType(n->Node.Binary.LeftOperand) == typeType(n->Node.Binary.RightOperand)){
					type = typeType(n->Node.Binary.LeftOperand);
					switch(n->Node.Binary.Operator) {
						case MINUS:
						case PLUS:
						case MULT:
						case DIV:
							return type;
						case OR:
						case AND:
						case EQ:
						case LT:
						case LE:
							return BOOL;
					}
				}
				else{
					printf("kBinary: Error\n");
					return ERROR;
				}
				break;
			case kIntConst:
				printf("kInt\n");
				return INT;
			case kBoolConst:
				printf("kBol\n");
				return BOOL;
			case kStringConst:
				printf("kString\n");
				return STRING;
			case kFuncCallExpr:
				printf("kFuncCallExpr\n");
				if(typeType(n->Node.FuncCallExpr.Actuals) == VOID) {
					strcpy(str, "> ");
					for(tmp = n->Node.FuncCallExpr.Actuals; tmp != NULL; tmp = tmp->Node.Actual.Next){
						type = typeType(tmp->Node.Actual.Expr);
						if(type != VOID) {
							// MIGHT BE SEGFAULT!!!
							strcat(str, varType(typeType(tmp->Node.Actual.Expr)));
							strcat(str, " x ");
						}
					}
					printf("Symtab_get(%s): %s\n", n->Node.FuncCallExpr.FuncName, (char *)symtab_get(globalSymbols, n->Node.FuncCallExpr.FuncName));
					printf("actuals are done: %s\n", str);
					tmpStr = strchr((char *)symtab_get(globalSymbols, n->Node.FuncCallExpr.FuncName),'>');
					printf("After strchar %s\n", tmpStr);
					if(strncmp(str, tmpStr, strlen(tmpStr)) == 0){
						printf("strchr crashes us\n");
						strcpy(str, (char *)symtab_get(globalSymbols, n->Node.FuncCallExpr.FuncName));
						if(str != NULL){
							printf("%s\n %c\n", str, str[0]);
							switch(str[0]){
								case 'v':
									return VOID;
									break;
								case 'i':
									return INT;
									break;
								case 's':
									return STRING;
									break;
							}
							return type;
						}
					}
					else {
						printf("strncmp(%s, %s)\n", str, tmpStr); 
						return ERROR;
					}
				}
				else{
					printf("kFuncCallExpr: Error\n");
					return ERROR;
				}
				break;
			case kRValue:
				printf("kRValue\n");
				return (eType)symtab_get(current, n->Node.RValue.Id);
				break;
		}
	}
	else
		return VOID;
	return ERROR;
}

void typeAnalyse(t_tree root){
	eType ret = typeType(root);
	printf("\n\n%s\n\n", varType(ret));
}
