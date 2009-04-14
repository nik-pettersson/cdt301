/*
 * prog		->	func funclst
 */
int Prog() {
	Func();
	Funclst();
	return 0;
}

/*
 * funclst	->	func funclst
 *					|		e
 */
int Funclst() {
	return 0;
}

/*
 * func		->	type ID (params) body
 */
int Func() {
	return 0;
}

/*
 * params	->	VOID
 *				|		INT ID optpar
 */
int Params() {
	return 0;
}

/*
 * optpar	->	, INT ID optpar
 *				|		e
 */
int Optparams() {
	return 0;
}

/*
 * type		->	VOID
 *				|		INT
 */
int Type() {
	return 0;
}

/*
 * body		->	{ decl stmts }
 */
int Body() {
	return 0;
}

/*
 * decl		->	INT ID optpar ; decl
 *				|		e
 */
int Decl() {
	return 0;
}

/*
 * stmts	->	stmt stmts
 *				|		stmt
 */
int Stmts() {
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
int Stmt() {
	return 0;
}

/*
 * if			->	IF ( expr ) { stmts } else
 *				|		IF ( expr ) stmt else
 */
int If() {
	return 0;
}

/*
 * else		->	ELSE { stmts }
 *				|		ELSE stmt
 *				|		e
 */
int Else() {
	return 0;
}

/*
 * while	->	WHILE ( expr ) { stmts }
 *				|		WHILE ( expr ) stmt
 */
int While() {
	return 0;
}

/*
 * assign	->	ID ASSIGNOP expr
 */
int Assign() {
	return 0;
}

/*
 * call		->	ID ( expr optexpr )
 *				|		ID ( )
 *				|		ID
 */
int Call() {
	return 0;
}

/*
 * optexpr	->	, expr
 *					|		e
 */
int Optexpr() {
	return 0;
}

/*
 * expr		->	term exprs
 */
int Expr() {
	return 0;
}

/*
 * exprs	->	RELOP term exprs
 *				|		e
 */
int Exprs() {
	return 0;
}

/*
 * term		->	factor terms
 */
int Term() {
	return 0;
}

/*
 * terms		->	ADDOP factor terms
 *					|		SUBOP factor terms
 *					|		e
 */
int Terms() {
	return 0;
}

/*
 * factor	->	fac	factors
 */
int Factor() {
	return 0;
}

/*
 * factors	->	MULOP fac factors
 *					|		DIVOP fac factors
 *					|		e
 */
int Factors() {
	return 0;
}

/*
 * fac	->	! fac
 *			|		call
 *			|		NUM
 *			|		( expr )
 */
int Fac() {
	return 0;
}

