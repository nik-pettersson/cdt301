#include "scanner.h"
/*
 * prog		->	func funclst
 */
int Prog(int *tok, int *val);

/*
 * funclst	->	func funclst
 *					|		e
 */
int Funclst(int *tok, int *val);

/*
 * func		->	type ID (params body
 */
int Func(int *tok, int *val);

/*
 * params	->	VOID )
 *				|		INT ID optpar
 */
int Params(int *tok, int *val);

/*
 * optpar	->	, INT ID optpar
 *				|		)
 */
int Optparams(int *tok, int *val);

/*
 * type		->	VOID
 *				|		INT
 */
int Type(int *tok, int *val);

/*
 * body		->	{ decl stmts
 */
int Body(int *tok, int *val);

/*
 * decl		->	INT ID optpar ; decl
 *				|		e
 */
int Decl(int *tok, int *val);

/*
 * stmts	->	stmt stmts
 *				|		stmt }
 */
int Stmts(int *tok, int *val);

/*
 * stmt		->	if
 *				|		while
 *				|		assign ;
 *				|		call ;
 *				|		WRITE expr ;
 *				|		READ ID ;
 *				|		RETURN expr ;
 */
int Stmt(int *tok, int *val);

/*
 * if			->	IF ( expr ) { stmts } else
 *				|		IF ( expr ) stmt else
 */
int If(int *tok, int *val);

/*
 * else		->	ELSE { stmts }
 *				|		ELSE stmt
 *				|		e
 */
int Else(int *tok, int *val);

/*
 * while	->	WHILE ( expr ) { stmts }
 *				|		WHILE ( expr ) stmt
 */
int While(int *tok, int *val);

/*
 * assign	->	ID ASSIGNOP expr
 */
int Assign(int *tok, int *val);

/*
 * call		->	ID ( expr optexpr )
 *				|		ID ( )
 *				|		ID
 */
int Call(int *tok, int *val);

/*
 * optexpr	->	, expr
 *					|		e
 */
int Optexpr(int *tok, int *val);

/*
 * expr		->	term exprs
 */
int Expr(int *tok, int *val);

/*
 * exprs	->	RELOP term exprs
 *				|		e
 */
int Exprs(int *tok, int *val);

/*
 * term		->	factor terms
 */
int Term(int *tok, int *val);

/*
 * terms		->	ADDOP factor terms
 *					|		SUBOP factor terms
 *					|		e
 */
int Terms(int *tok, int *val);

/*
 * factor	->	fac	factors
 */
int Factor(int *tok, int *val);

/*
 * factors	->	MULOP fac factors
 *					|		DIVOP fac factors
 *					|		e
 */
int Factors(int *tok, int *val);

/*
 * fac	->	! fac
 *			|		call
 *			|		NUM
 *			|		( expr )
 */
int Fac(int *tok, int *val);

