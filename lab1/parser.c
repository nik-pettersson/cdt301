#include "parser.h"

#define ACK_IDENT -2

char gstring[64], gassign[64];
FILE *gfile = NULL;
int glabel = 0;

/*
 * prog		->	func funclst
 */
int Prog(char *filename) {
	int acc;
	int tok, val;

	if(filename == NULL) {
		return -1;
	}

	gfile = fopen(filename, "w");

	printf("+++ Prog +++\n");
	getNextToken(&tok, &val, gstring);
	if((acc = Func(&tok, &val))) {
		acc = Funclst(&tok, &val);
	}
	printf("\n--- Prog (%d) ---\n", acc  );
	fclose(gfile);
	return acc;
}

/*
 * funclst	->	func funclst
 *					|		e
 */
int Funclst(int *tok, int *val) {
	int acc;

	printf("+++ Funclst +++\n");
	switch(*tok) {
		case END:
			acc = TRUE;
			break;
		default:
			acc = Func(tok, val);
	}
	printf("\n--- Funclst (%d) ---\n", acc  );
	return acc;
}

/*
 * func		->	type ID (params body
 */
int Func(int *tok, int *val) {
	int acc;
	
	printf("+++ Func +++\n");
	if(Type(tok, val)) {
		switch(*tok) {
			case ID:
				fprintf(gfile, "label %s\n", gstring);
				getNextToken(tok, val, gstring);
				if(*tok == LEFTPARENTHESIS) {
					getNextToken(tok, val, gstring);
					if(Params(tok, val)) {
						acc = Body(tok, val);
					}
					else {
						acc = FALSE;
					}
				}
				else {
					acc = FALSE;
				}
				break;
			default:
				acc = FALSE;
				break;
		}
	}

	fprintf(gfile, "return\n");
	printf("\n--- Func (%d) ---\n", acc  );
	return acc;
}

/*
 * params	->	VOID )
 *				|		INT ID optpar
 */
int Params(int *tok, int *val) {
	int acc;

	printf("+++ Params +++\n");
	switch(*tok) {
		case VOID:
			getNextToken(tok, val, gstring);
			if(*tok == RIGHTPARENTHESIS) {
				acc = TRUE;
				getNextToken(tok, val, gstring);
			}
			else {
				acc = FALSE;
			}
			break;
		case INT:
			getNextToken(tok, val, gstring);
			if(*tok == ID) {
				getNextToken(tok, val, gstring);
				acc = Optparams(tok, val);
			}
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Params (%d) ---\n", acc  );
	return acc;
}

/*
 * optpar	->	, INT ID optpar
 *				|		)
 */
int Optparams(int *tok, int *val) {
	int acc;

	printf("+++ Optparams +++\n");
	switch(*tok) {
		case RIGHTPARENTHESIS:
			acc = TRUE;
			getNextToken(tok, val, gstring);
			break;
		case COMMA:
			getNextToken(tok, val, gstring);
			if(*tok == INT) {
				getNextToken(tok, val, gstring);
				if(*tok == ID) {
					getNextToken(tok, val, gstring);
					Optparams(tok, val);
				}
				else {
					acc = FALSE;
				}
			}
			else {
				acc = FALSE;
			}
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Optparams (%d) ---\n", acc  );
	return acc;
}

/*
 * type		->	VOID
 *				|		INT
 */
int Type(int *tok, int *val) {
	int acc;

	printf("+++ Type +++\n");

	switch(*tok) {
		case VOID:
		case INT:
			getNextToken(tok, val, gstring);
			acc = TRUE;
			break;
		default:
			acc = FALSE;
			break;
	}
	printf("\n--- Type (%d) ---\n", acc  );
	return acc;
}

/*
 * body		->	{ decl stmts
 */
int Body(int *tok, int *val) {
	int acc;

	printf("+++ Body +++\n");
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, gstring);
			if(Decl(tok, val)) {
				acc = Stmts(tok, val);
			}
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Body (%d) ---\n", acc  );
	return acc;
}

/*
 * decl		->	INT ID decls decl
 *				|		e
 */
int Decl(int *tok, int *val) {
	int acc;

	printf("+++ Decl +++\n");
	switch(*tok) {
		case INT:
			getNextToken(tok, val, gstring);
			if(*tok == ID) {
				fprintf(gfile, "declare %s\n", gstring);
				getNextToken(tok, val, gstring);
				if(Decls(tok, val)) {
					acc = Decl(tok, val);
				}
				else {
					acc = FALSE;
				}
			}
			else {
				acc = FALSE;
			}
			break;
		default:
			acc = TRUE; // Not a declaration (matches epsilon)
			break;
	}

	printf("\n--- Decl (%d) ---\n", acc  );
	return acc;
}

/*
 * decls	->	, ID decls
 * 				|		;
 */
int Decls(int *tok, int *val) {
	int acc;

	printf("+++ Decls +++\n");
	switch(*tok) {
		case COMMA:
			getNextToken(tok, val, gstring);
			if(*tok == ID) {
				fprintf(gfile, "declare %s\n", gstring);
				getNextToken(tok, val, gstring);
				acc = Decls(tok, val);
			}
			else {
				acc = FALSE;
			}
			break;
		case SEMICOLON:
			getNextToken(tok, val, gstring);
			acc = TRUE;
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Decls (%d) ---\n", acc  );
	return acc;
}

/*
 * stmts	->	stmt stmts
 *				|		stmt }
 */
int Stmts(int *tok, int *val) {
	int acc;

	printf("+++ Stmts +++\n");
	if(Stmt(tok, val)) {
		switch(*tok) {
			case RIGHTBRACE:
				getNextToken(tok, val, gstring);
				acc = TRUE;
				break;
			default:
				acc = Stmts(tok, val);
				break;
		}
	}
	else {
		acc = FALSE;
	}

	printf("\n--- Stmts (%d) ---\n", acc  );
	return acc;
}

/*
 * stmt		->	IF if
 *				|		WHILE while
 *				|		ID call ;
 *				|		WRITE expr ;
 *				|		READ ID ;
 *				|		RETURN expr ;
 */
int Stmt(int *tok, int *val) {
	int acc;
	char tmp[64];

	printf("+++ Stmt +++\n");
	// Consume the first token for the corresponding rules.
	// This results in a slight deviation from the grammar.
	switch(*tok) {
		case IF:
			getNextToken(tok, val, gstring);
			acc = If(tok, val);
			break;
		case WHILE:
			getNextToken(tok, val, gstring);
			acc = While(tok, val);
			break;
		case WRITE:
			getNextToken(tok, val, gstring);
			if(!Expr(tok, val))
				acc = FALSE;
			else if(*tok != SEMICOLON) {
				acc = FALSE;
			}
			else {
				fprintf(gfile, "write\n");
				fprintf(gfile, "pop\n");
				getNextToken(tok, val, gstring);
				acc = TRUE;
			}
			break;
		case READ:
			getNextToken(tok, val, gstring);
			if(*tok == ID) {
				fprintf(gfile, "lvalue %s\n", gstring);
				fprintf(gfile, "read\n");
				fprintf(gfile, ":=\n");
				getNextToken(tok, val, gstring);
				if(*tok != SEMICOLON)
					acc = FALSE;
				else {
					getNextToken(tok, val, gstring);
					acc = TRUE;
				}
			}
			break;
		case RETURN:
			fprintf(gfile, "lvalue RET_VAL_PLACE\n");
			getNextToken(tok, val, gstring);
			if(!Expr(tok, val)) {
				printf("NOT VALID EXPR¿\n");
				acc = FALSE;
			}
			else if(*tok != SEMICOLON) {
				printf("COULD NOT MATCH SEMICOLON\n");
				acc = FALSE;
			}
			else {
				fprintf(gfile, ":=\n");
				fprintf(gfile, "return\n");
				getNextToken(tok, val, gstring);
				acc = TRUE;
			}
			break;
		case ID:
			strncpy(tmp, gstring, 64);
			strncpy(gassign, gstring, 64);
			getNextToken(tok, val, gstring);
			acc = Call(tok, val);
			if(!acc) {
				acc = FALSE;
			}
			else if(*tok != SEMICOLON) {
				acc = FALSE;
			}
			else {
				if(acc == ACK_IDENT) // hotfix to determine 'assign' or 'call'
					fprintf(gfile, ":=\n");
				else {
					fprintf(gfile, "call %s\n", tmp);
					fprintf(gfile, "pop %d\n", acc);
				}
				getNextToken(tok, val, gstring);
				acc = TRUE;
			}
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Stmt (%d) ---\n", acc  );
	return acc;
}

/*
 * if			->	( expr ) { stmts else
 *				|		( expr ) stmt else
 */
int If(int *tok, int *val) {
	int acc;
	int label;

	glabel += 2;
	label = glabel;

	printf("+++ If +++\n");
	if(*tok != LEFTPARENTHESIS)
		return FALSE;
	getNextToken(tok, val, gstring);
	if(!Expr(tok, val))
		return FALSE;
	if(*tok != RIGHTPARENTHESIS)
		return FALSE;
	
	getNextToken(tok, val, gstring);
	fprintf(gfile, "gofalse L%d\n", label-1);
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, gstring);
			if(!Stmts(tok, val))
				return FALSE;
			fprintf(gfile, "goto L%d\n", label);
			fprintf(gfile, "label L%d\n", label-1);
			acc = Else(tok, val);
			fprintf(gfile, "label L%d\n", label);
			break;
		default:
			if(!Stmt(tok, val))
				return FALSE;
			fprintf(gfile, "goto L%d\n", label);
			fprintf(gfile, "label L%d\n", label-1);
			acc = Else(tok, val);
			fprintf(gfile, "label L%d\n", label);
			break;
	}

	printf("\n--- If (%d) ---\n", acc  );
	return acc;
}

/*
 * else		->	ELSE { stmts
 *				|		ELSE stmt
 *				|		e
 */
int Else(int *tok, int *val) {
	int acc;

	printf("+++ Else +++\n");
	if(*tok != ELSE) {
		printf("\n--- Else (%d) ---\n", acc  );
		return TRUE;
	}

	getNextToken(tok, val, gstring);
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, gstring);
			acc = Stmts(tok, val);
			break;
		default:
			acc = Stmt(tok, val);
			break;
	}

	printf("\n--- Else (%d) ---\n", acc  );
	return acc;
}

/*
 * while	->	( expr ) { stmts
 *				|		( expr ) stmt
 */
int While(int *tok, int *val) {
	int acc;
	int label;

	glabel += 2;
	label = glabel;

	printf("+++ While +++\n");
	if(*tok != LEFTPARENTHESIS) {
		printf("\n--- While (%d) ---\n", acc  );
		return FALSE;
	}

	fprintf(gfile, "label L%d\n", label-1);
	getNextToken(tok, val, gstring);
	if(!Expr(tok, val)) {
		printf("\n--- While (%d) ---\n", acc  );
		return FALSE;
	}
	if(*tok != RIGHTPARENTHESIS) {
		printf("\n--- While (%d) ---\n", acc  );
		return FALSE;
	}

	fprintf(gfile, "gofalse L%d\n", label);
	getNextToken(tok, val, gstring);
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, gstring);
			acc = Stmts(tok, val);
			fprintf(gfile, "goto L%d\n", label-1);
			fprintf(gfile, "label L%d\n", label);
			break;
		default:
			acc = Stmt(tok, val);
			fprintf(gfile, "goto L%d\n", label-1);
			fprintf(gfile, "label L%d\n", label);
			break;
	}

	printf("\n--- While (%d) ---\n", acc  );
	return acc;
}

/*
 * call		->	( expr optexpr )
 *				|		( )
 *				|		ASSIGNOP expr
 */
int Call(int *tok, int *val) {
	int acc;

	printf("+++ Call +++\n");
	switch(*tok) {
		case LEFTPARENTHESIS:
			fprintf(gfile, "push 0\n");
			getNextToken(tok, val, gstring);
			switch(*tok) {
				case RIGHTPARENTHESIS:
					getNextToken(tok, val, gstring);
					acc = TRUE;
					break;
				default:
					if(!Expr(tok, val)) {
						acc = FALSE;
					}
					else {
						if(!(acc = Optexpr(tok, val))) {
							acc = FALSE;
						}
						else if(*tok != RIGHTPARENTHESIS) {
							acc = FALSE;
						}
						else {
							getNextToken(tok, val, gstring);
						}
						
					}
					break;
			}
			break;
		case ASSIGNOP:
			fprintf(gfile, "lvalue %s\n", gassign);
			getNextToken(tok, val, gstring);
			acc = Expr(tok, val);
			if(acc)
				acc = ACK_IDENT;
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Call (%d) ---\n", acc  );
	return acc;
}

/*
 * optexpr	->	, expr optexpr
 *					|		e
 */
int Optexpr(int *tok, int *val) {
	int acc = 0;

	printf("+++ Optexpr +++\n");
	switch(*tok) {
		case COMMA:
			getNextToken(tok, val, gstring);
			if(!Expr(tok, val)) {
				acc = FALSE;
			}
			else if ((acc = Optexpr(tok, val)) == FALSE){
				acc = FALSE;
			}
			else {
				acc++;
			}
			break;
		default:
			acc = TRUE;
			break;
	}

	printf("\n--- Optexpr (%d) ---\n", acc  );
	return acc;
}

/*
 * expr		->	term exprs
 */
int Expr(int *tok, int *val) {
	int acc;

	printf("+++ Expr +++\n");
	if(Term(tok, val))
		acc = Exprs(tok, val);
	else
		acc = FALSE;

	printf("\n--- Expr (%d) ---\n", acc  );
	return acc;
}

/*
 * exprs	->	RELOP term exprs
 *				|		e
 */
int Exprs(int *tok, int *val) {
	int acc;
	int op;

	printf("+++ Exprs +++\n");
	if(*tok == RELATIONOP) {
		op = *val;
		getNextToken(tok, val, gstring);
		if(Term(tok, val)) {
			acc = Exprs(tok, val);
			switch(op) {
				case EQUAL:
					fprintf(gfile, "==\n");
					break;
				case BIGGER:
					fprintf(gfile, ">=\n");
					break;
				case SMALLER:
					fprintf(gfile, "<=\n");
					break;
				case NOTEQUAL:
					fprintf(gfile, "!=\n");
					break;
			}
		}
		else
			acc = FALSE;
	}
	else
		acc = TRUE;

	printf("\n--- Exprs (%d) ---\n", acc  );
	return acc;
}

/*
 * term		->	factor terms
 */
int Term(int *tok, int *val) {
	int acc;

	printf("+++ Term +++\n");
	if(Factor(tok, val))
		acc = Terms(tok, val);
	else
		acc = FALSE;

	printf("\n--- Term (%d) ---\n", acc  );
	return acc;
}

/*
 * terms		->	ADDOP factor terms
 *					|		SUBOP factor terms
 *					|		e
 */
int Terms(int *tok, int *val) {
	int acc;

	printf("+++ Terms +++\n");
	switch(*tok) {
		case PLUSOP:
			getNextToken(tok, val, gstring);
			if(Factor(tok, val)) {
				acc = Terms(tok, val);
				fprintf(gfile, "+\n");
			}
			else
				acc = FALSE;
			break;
		case MINUSOP:
			getNextToken(tok, val, gstring);
			if(Factor(tok, val)) {
				acc = Terms(tok, val);
				fprintf(gfile, "-\n");
			}
			else
				acc = FALSE;
			break;
		default:
			acc = TRUE;
			break;
	}

	printf("\n--- Terms (%d) ---\n", acc  );
	return acc;
}

/*
 * factor	->	fac	factors
 */
int Factor(int *tok, int *val) {
	int acc;

	printf("+++ Factor +++\n");
	if(!Fac(tok, val))
		acc = FALSE;
	else
		acc = Factors(tok, val);

	printf("\n--- Factors (%d) ---\n", acc  );
	return acc;
}

/*
 * factors	->	MULOP fac factors
 *					|		DIVOP fac factors
 *					|		e
 */
int Factors(int *tok, int *val) {
	int acc;

	printf("+++ Factors +++\n");
	switch(*tok) {
		case MULTOP:
			getNextToken(tok, val, gstring);
			if(!Fac(tok, val))
				acc = FALSE;
			else {
				acc = Factors(tok, val);
				fprintf(gfile, "*\n");
			}
			break;
		case DIVOP:
			getNextToken(tok, val, gstring);
			if(!Fac(tok, val))
					acc = FALSE;
			else {
				acc = Factors(tok, val);
				fprintf(gfile, "/\n");
			}
			break;
		default:
			acc = TRUE;
			break;
	}

	printf("\n--- Factors (%d) ---\n", acc  );
	return acc;
}

/*
 * fac	->	! fac
 *			|		ID call
 *			|		NUM
 *			|		( expr )
 */
int Fac(int *tok, int *val) {
	int acc;
	char tmp[64];

	printf("+++ Fac +++\n");
	switch(*tok) {
		case NOTOP:
			getNextToken(tok, val, gstring);
			acc = Fac(tok, val);
			fprintf(gfile, "not\n");
			break;
		case ID:
			strncpy(tmp, gstring, 64);
			getNextToken(tok, val, gstring);
			if(*tok == LEFTPARENTHESIS || *tok == ASSIGNOP){
				acc = Call(tok, val);
				if(acc == ACK_IDENT) // hotfix to determine 'assign' or 'call'
					fprintf(gfile, ":=\n");
				else
					fprintf(gfile, "call %s\n", tmp);
					fprintf(gfile, "pop %d\n", acc);
			}
			else {
				fprintf(gfile, "rvalue %s\n", tmp);
				acc = TRUE;
			}
			break;
		case NUM:
			fprintf(gfile, "push %d\n", *val);
			getNextToken(tok, val, gstring);
			acc = TRUE;
			break;
		case LEFTPARENTHESIS:
			getNextToken(tok, val, gstring);
			if(!Expr(tok, val))
				acc = FALSE;
			else if(*tok == RIGHTPARENTHESIS) {
				getNextToken(tok, val, gstring);
				acc = TRUE;
			}
			else
				acc = FALSE;
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- fac (%d) ---\n", acc  );
	return acc;
}

