int nextToken(int *token, int *value) {
	getNextToken(token, value, NULL);
	if(*token == ERROR)
		return FALSE;
	return TRUE;
}

int matchNextToken(int token) {
	int tok, val, ret;

	if(nextToken(&tok, &val, NULL)) {
		if(tok == token) {
			return TRUE;
		}
	}

	return FALSE;
}

/*
 * prog		->	func funclst
 */
int Prog(int *tok, int *val) {
	int acc;
	getNextToken(tok, val, NULL);
	if(Func(tok, val)) {
		acc = Funclst(tok, val);
	}
	return acc;
}

/*
 * funclst	->	func funclst
 *					|		e
 */
int Funclst(int *tok, int *val) {
	int acc;

	switch(*tok) {
		case END:
			acc = TRUE;
			break;
		default:
			getNextToken(tok, val, NULL);
			acc = Func(tok, val);
	}
	return acc;
}

/*
 * func		->	type ID (params body
 */
int Func(int *tok, int *val) {
	int acc;
	
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

	return acc;
}

/*
 * params	->	VOID )
 *				|		INT ID optpar
 */
int Params(int *tok, int *val) {
	int acc;

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

	return acc;
}

/*
 * optpar	->	, INT ID optpar
 *				|		)
 */
int Optparams(int *tok, int *val) {
	int acc;

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

	return acc;
}

/*
 * type		->	VOID
 *				|		INT
 */
int Type(int *tok, int *val) {
	int acc;

	switch(*tok) {
		case VOID:
		case INT:
			acc = TRUE;
			break;
		default:
			acc = FALSE;
			break;
	}
	return acc;
}

/*
 * body		->	{ decl stmts
 */
int Body(int *tok, int *val) {
	int acc = FALSE;

	if(matchNextToken(LEFTBRACE)) {
		if(decl())
			acc = Stmts();
	}
	return acc;
}

/*
 * decl		->	INT ID optpar ; decl
 *				|		e
 */
int Decl(int *tok, int *val) {
	int tok, val;
	int acc;

	if(ma

	return acc;
}

/*
 * stmts	->	stmt stmts
 *				|		stmt }
 */
int Stmts(int *tok, int *val) {
	return 0;
}

/*
 * stmt		->	if
 *				|		while
 *				|		assign ;
 *				|		call ;
 *				|		WRITE expr ;
 *				|		READ ID ;
 *				|		RETURN expr ;
 */
int Stmt(int *tok, int *val) {
	return 0;
}

/*
 * if			->	IF ( expr ) { stmts } else
 *				|		IF ( expr ) stmt else
 */
int If(int *tok, int *val) {
	return 0;
}

/*
 * else		->	ELSE { stmts }
 *				|		ELSE stmt
 *				|		e
 */
int Else(int *tok, int *val) {
	return 0;
}

/*
 * while	->	WHILE ( expr ) { stmts }
 *				|		WHILE ( expr ) stmt
 */
int While(int *tok, int *val) {
	return 0;
}

/*
 * assign	->	ID ASSIGNOP expr
 */
int Assign(int *tok, int *val) {
	return 0;
}

/*
 * call		->	ID ( expr optexpr )
 *				|		ID ( )
 *				|		ID
 */
int Call(int *tok, int *val) {
	return 0;
}

/*
 * optexpr	->	, expr
 *					|		e
 */
int Optexpr(int *tok, int *val) {
	return 0;
}

/*
 * expr		->	term exprs
 */
int Expr(int *tok, int *val) {
	return 0;
}

/*
 * exprs	->	RELOP term exprs
 *				|		e
 */
int Exprs(int *tok, int *val) {
	return 0;
}

/*
 * term		->	factor terms
 */
int Term(int *tok, int *val) {
	return 0;
}

/*
 * terms		->	ADDOP factor terms
 *					|		SUBOP factor terms
 *					|		e
 */
int Terms(int *tok, int *val) {
	return 0;
}

/*
 * factor	->	fac	factors
 */
int Factor(int *tok, int *val) {
	return 0;
}

/*
 * factors	->	MULOP fac factors
 *					|		DIVOP fac factors
 *					|		e
 */
int Factors(int *tok, int *val) {
	return 0;
}

/*
 * fac	->	! fac
 *			|		call
 *			|		NUM
 *			|		( expr )
 */
int Fac(int *tok, int *val) {
	return 0;
}

