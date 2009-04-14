#include "scanner.h"
/*
 * prog		->	func funclst
 */
int Prog();

/*
 * funclst	->	func funclst
 *					|		e
 */
int Funclst();

/*
 * func		->	type ID (params) body
 */
int Func();

/*
 * params	->	VOID
 *				|		INT ID optpar
 */
int Params();

/*
 * optpar	->	, INT ID optpar
 *				|		e
 */
int Optparams();

/*
 * type		->	VOID
 *				|		INT
 */
int Type();

/*
 * body		->	{ decl stmts }
 */
int Body();

/*
 * decl		->	INT ID optpar ; decl
 *				|		e
 */
int Decl();

/*
 * stmts	->	stmt stmts
 *				|		stmt
 */
int Stmts();

/*
 * stmt		->	if
 *				|		while
 *				|		assign ;
 *				|		call ;
 *				|		WRITE expr ;
 *				|		READ ID ;
 *				|		RETURN expr ;
 */
int Stmt();

/*
 * if			->	IF ( expr ) { stmts } else
 *				|		IF ( expr ) stmt else
 */
int If();

/*
 * else		->	ELSE { stmts }
 *				|		ELSE stmt
 *				|		e
 */
int Else();

/*
 * while	->	WHILE ( expr ) { stmts }
 *				|		WHILE ( expr ) stmt
 */
int While();

/*
 * assign	->	ID ASSIGNOP expr
 */
int Assign();

/*
 * call		->	ID ( expr optexpr )
 *				|		ID ( )
 *				|		ID
 */
int Call();

/*
 * optexpr	->	, expr
 *					|		e
 */
int Optexpr();

/*
 * expr		->	term exprs
 */
int Expr();

/*
 * exprs	->	RELOP term exprs
 *				|		e
 */
int Exprs();

/*
 * term		->	factor terms
 */
int Term();

/*
 * terms		->	ADDOP factor terms
 *					|		SUBOP factor terms
 *					|		e
 */
int Terms();

/*
 * factor	->	fac	factors
 */
int Factor();

/*
 * factors	->	MULOP fac factors
 *					|		DIVOP fac factors
 *					|		e
 */
int Factors();

/*
 * fac	->	! fac
 *			|		call
 *			|		NUM
 *			|		( expr )
 */
int Fac();

