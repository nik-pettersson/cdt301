#include "parser.h"

/*
 * prog		->	func funclst
 */
int Prog(int *tok, int *val) {
	int acc;
	printf("+++ Prog +++\n");
	getNextToken(tok, val, NULL);
	if(Func(tok, val)) {
		acc = Funclst(tok, val);
	}
	printf("\n--- Prog ---\n");
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
			getNextToken(tok, val, NULL);
			acc = Func(tok, val);
	}
	printf("\n--- Funclst ---\n");
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
				getNextToken(tok, val, NULL);
				if(*tok == LEFTPARENTHESIS) {
					getNextToken(tok, val, NULL);
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

	printf("\n--- Func ---\n");
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
			getNextToken(tok, val, NULL);
			if(*tok == RIGHTPARENTHESIS) {
				acc = TRUE;
				getNextToken(tok, val, NULL);
			}
			else {
				acc = FALSE;
			}
			break;
		case INT:
			getNextToken(tok, val, NULL);
			if(*tok == ID) {
				getNextToken(tok, val, NULL);
				acc = Optparams(tok, val);
			}
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Params ---\n");
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
			getNextToken(tok, val, NULL);
			break;
		case COMMA:
			getNextToken(tok, val, NULL);
			if(*tok == INT) {
				getNextToken(tok, val, NULL);
				if(*tok == ID) {
					getNextToken(tok, val, NULL);
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

	printf("\n--- Optparams ---\n");
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
			acc = TRUE;
			break;
		default:
			acc = FALSE;
			break;
	}
	printf("\n--- Type ---\n");
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
			getNextToken(tok, val, NULL);
			if(Decl(tok, val)) {
				acc = Stmts(tok, val);
			}
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Body ---\n");
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
			getNextToken(tok, val, NULL);
			if(*tok == ID) {
				getNextToken(tok, val, NULL);
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

	printf("\n--- Decl ---\n");
	return acc;
}

/*
 * decls	->	, INT ID decls
 * 				|		;
 */
int Decls(int *tok, int *val) {
	int acc;

	printf("+++ Decls +++\n");
	switch(*tok) {
		case COMMA:
			getNextToken(tok, val, NULL);
			if(*tok == INT) {
				getNextToken(tok, val, NULL);
				if(*tok == ID) {
					getNextToken(tok, val, NULL);
					acc = Decls(tok, val);
				}
				else {
					acc = FALSE;
				}
			}
			else {
				acc = FALSE;
			}
			break;
		case SEMICOLON:
			acc = TRUE;
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Decls ---\n");
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
				getNextToken(tok, val, NULL);
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

	printf("\n--- Stmts ---\n");
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

	printf("+++ Stmt +++\n");
	// Consume the first token for the corresponding rules.
	// This results in a slight deviation from the grammar.
	switch(*tok) {
		case IF:
			getNextToken(tok, val, NULL);
			acc = If(tok, val);
			break;
		case WHILE:
			getNextToken(tok, val, NULL);
			acc = While(tok, val);
			break;
		case WRITE:
			getNextToken(tok, val, NULL);
			acc = Expr(tok, val);
			break;
		case READ:
			getNextToken(tok, val, NULL);
			if(*tok == ID) {
				getNextToken(tok, val, NULL);
				acc = TRUE;
			}
			break;
		case RETURN:
			getNextToken(tok, val, NULL);
			acc = Expr(tok, val);
			break;
		case ID:
			getNextToken(tok, val, NULL);
			acc = Call(tok, val);
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Stmt ---\n");
	return acc;
}

/*
 * if			->	( expr ) { stmts else
 *				|		( expr ) stmt else
 */
int If(int *tok, int *val) {
	int acc;

	printf("+++ If +++\n");
	if(*tok != LEFTPARENTHESIS)
		return FALSE;
	getNextToken(tok, val, NULL);
	if(!Expr(tok, val))
		return FALSE;
	if(*tok != RIGHTPARENTHESIS)
		return FALSE;
	
	getNextToken(tok, val, NULL);
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, NULL);
			if(!Stmts(tok, val))
				return FALSE;
			acc = Else(tok, val);
			break;
		default:
			if(!Stmt(tok, val))
				return FALSE;
			acc = Else(tok, val);
			break;
	}

	printf("\n--- If ---\n");
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
		printf("\n--- Else ---\n");
		return TRUE;
	}

	getNextToken(tok, val, NULL);
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, NULL);
			acc = Stmts(tok, val);
			break;
		default:
			acc = Stmt(tok, val);
			break;
	}

	printf("\n--- Else ---\n");
	return acc;
}

/*
 * while	->	( expr ) { stmts
 *				|		( expr ) stmt
 */
int While(int *tok, int *val) {
	int acc;

	printf("+++ While +++\n");
	if(*tok != LEFTPARENTHESIS) {
		printf("\n--- While ---\n");
		return FALSE;
	}

	getNextToken(tok, val, NULL);
	if(!Expr(tok, val)) {
		printf("\n--- While ---\n");
		return FALSE;
	}
	if(*tok != RIGHTPARENTHESIS) {
		printf("\n--- While ---\n");
		return FALSE;
	}

	getNextToken(tok, val, NULL);
	switch(*tok) {
		case LEFTBRACE:
			getNextToken(tok, val, NULL);
			acc = Stmts(tok, val);
			break;
		default:
			acc = Stmt(tok, val);
			break;
	}

	printf("\n--- While ---\n");
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
			getNextToken(tok, val, NULL);
			switch(*tok) {
				case RIGHTPARENTHESIS:
					getNextToken(tok, val, NULL);
					acc = TRUE;
					break;
				default:
					if(!Expr(tok, val))
						acc = FALSE;
					else 
						acc = Optexpr(tok, val);
					break;
			}
			break;
		case ASSIGNOP:
			getNextToken(tok, val, NULL);
			acc = Expr(tok, val);
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- Call ---\n");
	return acc;
}

/*
 * optexpr	->	, expr
 *					|		e
 */
int Optexpr(int *tok, int *val) {
	int acc;

	printf("+++ Optexpr +++\n");
	switch(*tok) {
		case COMMA:
			getNextToken(tok, val, NULL);
			acc = Expr(tok, val);
			break;
		default:
			acc = TRUE;
			break;
	}

	printf("\n--- Optexpr ---\n");
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

	printf("\n--- Expr ---\n");
	return acc;
}

/*
 * exprs	->	RELOP term exprs
 *				|		e
 */
int Exprs(int *tok, int *val) {
	int acc;

	printf("+++ Exprs +++\n");
	if(*tok == RELATIONOP) {
		getNextToken(tok, val, NULL);
		if(Term(tok, val))
			acc = Exprs(tok, val);
		else
			acc = FALSE;
	}
	else
		acc = TRUE;

	printf("\n--- Exprs ---\n");
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

	printf("\n--- Term ---\n");
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
			getNextToken(tok, val, NULL);
			if(Factor(tok, val))
				acc = Terms(tok, val);
			else
				acc = FALSE;
			break;
		case MINUSOP:
			getNextToken(tok, val, NULL);
			if(Factor(tok, val))
				acc = Terms(tok, val);
			else
				acc = FALSE;
			break;
		default:
			acc = TRUE;
			break;
	}

	printf("\n--- Terms ---\n");
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

	printf("\n--- Factors ---\n");
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
			getNextToken(tok, val, NULL);
			if(!Fac(tok, val))
				acc = FALSE;
			else
				acc = Factors(tok, val);
			break;
		case DIVOP:
			getNextToken(tok, val, NULL);
			if(!Fac(tok, val))
					acc = FALSE;
			else
				acc = Factors(tok, val);
			break;
		default:
			acc = TRUE;
			break;
	}

	printf("\n--- Factors ---\n");
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

	printf("+++ Fac +++\n");
	switch(*tok) {
		case NOTOP:
			getNextToken(tok, val, NULL);
			acc = Fac(tok, val);
			break;
		case ID:
			getNextToken(tok, val, NULL);
			acc = Call(tok, val);
			break;
		case NUM:
			getNextToken(tok, val, NULL);
			acc = TRUE;
			break;
		case LEFTPARENTHESIS:
			getNextToken(tok, val, NULL);
			if(!Expr(tok, val))
				acc = FALSE;
			else if(*tok == RIGHTPARENTHESIS) {
				getNextToken(tok, val, NULL);
				acc = TRUE;
			}
			else
				acc = FALSE;
			break;
		default:
			acc = FALSE;
			break;
	}

	printf("\n--- fac ---\n");
	return acc;
}

