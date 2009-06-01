#include "codeGen.h"

t_symtab *globalSymbols;
t_symtab *current;

eType codeType(t_tree n, tCode *c){
	IOffset * io;
	if(n != NULL){
		switch(n->Kind){
			case kProgram:
				printf("Program\n");
				codeType(n->Node.Program.Functions, c);
				break;
			case kFunction:
				printf("Function %s\n", n->Node.Function.Name);
				current = n->Node.Function.Symbols;
				io = (IOffset *)malloc(sizeof(IOffset));
				io->Offset = get_next_address(c);
				io->Type = n->Node.Function.Type;
				{
					IOffset *tmp = symtab_get(current, "10Return");
					printf("10Return: %d %d\n", tmp->Offset, tmp->Size);
					io->Size = tmp->Offset - 2;
				}
					printf("10Return: %d \n", io->Size);
				symtab_set(globalSymbols, n->Node.Function.Name, io);
				emit_fun_label(c, n->Node.Function.Name);
				emit_link(c);
				codeType(n->Node.Function.Variables, c);
				printf("Function variables %s\n", n->Node.Function.Name);
				codeType(n->Node.Function.Stmnts, c);
				printf("Function Stmnts %s\n", n->Node.Function.Name);
				emit_unlink(c);
				emit_rts(c);
				codeType(n->Node.Function.Next, c);
				break;
			case kVariable:
				printf("Variable\n");
				codeType(n->Node.Variable.Next, c);
				switch(n->Node.Variable.VarKind){
					case kFormal:
						break;
					case kLocal:
						switch(n->Node.Variable.Type){
							case INT:
								emit_decl(c, INT_SIZE, n->Node.Variable.Name);
								//add(n->Node.Variable.Name, ADRESS, list)
								break;
							case BOOL:
								emit_decl(c, BOOL_SIZE, n->Node.Variable.Name);
								break;
							case STRING:
								emit_decl(c, STRING_SIZE, n->Node.Variable.Name);
								break;
							case VOID:
								break;
							case ERROR:
								break;
						}
						break;
				}
				break;
			case kAssign:
				printf("Assign: %s\n", n->Node.Assign.Id);
				io = (IOffset *)symtab_get(current, n->Node.Assign.Id);
				emit_lval(c, io->Offset, n->Node.Assign.Id);
				codeType(n->Node.Assign.Expr, c);
				printf("Assign: expr  %s\n", n->Node.Assign.Id);
				emit_ass(c, io->Type);
				codeType(n->Node.Assign.Next, c);
				printf("Assign: Next %s\n", n->Node.Assign.Id);
				break;
			case kIf:
				printf("If\n");
				{
					tInstr *iBrf, *iBra;
					codeType(n->Node.If.Expr, c); // do expr
					iBrf = emit_brf(c);
					codeType(n->Node.If.Then, c);
					iBra = emit_bra(c);
					set_address(iBrf, get_next_address(c));
					codeType(n->Node.If.Else, c);
					set_address(iBra, get_next_address(c));
					codeType(n->Node.If.Next, c);
				}
				break;
			case kWhile:
				printf("While\n");
				{
					int compareAddr = get_next_address(c);
					tInstr *iBrf, *iBra;

					codeType(n->Node.While.Expr, c);
					iBrf = emit_brf(c);
					codeType(n->Node.While.Stmnt, c);
					iBra = emit_bra(c);
					set_address(iBra, compareAddr);
					set_address(iBrf, get_next_address(c));
					codeType(n->Node.While.Next, c);
				}
				break;
			case kRead:
				printf("Read\n");
				io = symtab_get(current, n->Node.Read.Id);
				emit_lval(c, io->Offset, n->Node.Read.Id);
				emit_read(c, io->Type);
				emit_ass(c, io->Type);
				codeType(n->Node.Read.Next, c);
				break;	
			case kWrite:
				{
					eType t;
					int i;
					printf("Write\n");
					t = codeType(n->Node.Write.Expr, c);
					emit_write(c, t);
					switch(t) {
						case INT:
						case BOOL:
							i = INT_SIZE;
							break;
						case STRING:
							i = STRING_SIZE;
							break;
						default:
							i = 0;
							break;
					}
					emit_pop(c, i);
					codeType(n->Node.Write.Next, c);
				}
				break;
			case kFuncCallStmnt:
				printf("FuncCallStmnt\n");
				io = symtab_get(globalSymbols, n->Node.FuncCallStmnt.FuncName);
				switch(io->Type) {
					case INT:
						emit_decl(c, INT_SIZE, n->Node.FuncCallStmnt.FuncName);
						break;
					case BOOL:
						emit_decl(c, BOOL_SIZE, n->Node.FuncCallStmnt.FuncName);
						break;
					case STRING:
						emit_decl(c, STRING_SIZE, n->Node.FuncCallStmnt.FuncName);
						break;
					default:
						break;
				}
				codeType(n->Node.FuncCallStmnt.Actuals, c);
				{
					tInstr *iBsr = emit_bsr(c);
					set_address(iBsr, io->Offset);
				}
				printf("io->Size = %d", io->Size);
				if(io->Size > 0)
					emit_pop(c, io->Size);
				codeType(n->Node.FuncCallStmnt.Next, c);
				break;
			case kReturn:
				printf("Return\n");
				io = (IOffset *)symtab_get(current, "10Return");
				emit_lval(c, io->Offset, "Return");
				codeType(n->Node.Return.Expr, c);
				emit_ass(c, io->Type);
				emit_unlink(c);
				emit_rts(c);
				codeType(n->Node.Return.Next, c);
				break;
			case kActual:
				printf("Actual\n");
				codeType(n->Node.Actual.Expr, c);
				codeType(n->Node.Actual.Next, c);
				break;
			case kUnary:
				printf("Unary\n");
				{
					eType t = codeType(n->Node.Unary.Expr, c);
					switch(n->Node.Unary.Operator) {
						case NOT:
							emit_not(c);
							break;
						case NEG:
							emit_neg(c);
							break;
					}
					return t;
				}
				break;
			case kBinary:
				printf("Binary\n");
				{
					eType t = codeType(n->Node.Binary.LeftOperand, c);
					codeType(n->Node.Binary.RightOperand, c);
					switch(n->Node.Binary.Operator) {
						case SUB:
							emit_sub(c);
							break;
						case PLUS:
							emit_add(c);
							break;
						case MULT:
							emit_mult(c);
							break;
						case DIV:
							emit_div(c);
							break;
						case OR:
							emit_or(c);
							t = BOOL;
							break;
						case AND:
							emit_and(c);
							t = BOOL;
							break;
						case EQ:
							emit_eq(c, t);
							t = BOOL;
							break;
						case LT:
							emit_lt(c, t);
							t = BOOL;
							break;
						case LE:
							emit_le(c, t);
							t = BOOL;
							break;
					}
					return t;
				}
				break;
			case kIntConst:
				printf("IntConst\n");
				emit_push_int(c, n->Node.IntConst.Value);
				return INT;
				break;
			case kBoolConst:
				printf("BoolConst\n");
				emit_push_bool(c, n->Node.BoolConst.Value);
				return BOOL;
				break;
			case kStringConst:
				printf("StringConst\n");
				emit_push_string(c, n->Node.StringConst.Value);
				return STRING;
				break;
			case kFuncCallExpr:
				printf("FuncCallExpr\n");
				io = symtab_get(globalSymbols, n->Node.FuncCallExpr.FuncName);
				switch(io->Type) {
					case INT:
						emit_decl(c, INT_SIZE, n->Node.FuncCallExpr.FuncName);
						break;
					case BOOL:
						emit_decl(c, BOOL_SIZE, n->Node.FuncCallExpr.FuncName);
						break;
					case STRING:
						emit_decl(c, STRING_SIZE, n->Node.FuncCallExpr.FuncName);
						break;
					default:
						break;
				}
				codeType(n->Node.FuncCallExpr.Actuals, c);
				{
					tInstr *iBsr = emit_bsr(c);
					set_address(iBsr, io->Offset);
				}
				printf("io->Size = %d", io->Size);
				if(io->Size > 0)
					emit_pop(c, io->Size);
				return io->Type;
				break;
			case kRValue:
				printf("RValue\n");
				io = symtab_get(current, n->Node.RValue.Id);
				emit_rval(c, io->Type, io->Offset, n->Node.RValue.Id);
				return io->Type;
				break;
		}
	}
	return VOID;
}

tCode * generateCode(t_tree root){
	offsetType(root);
	printf("Offset calculation done\n");
	tCode *c = code_buffer_create();
	codeType(root, c);
	return c;
}
