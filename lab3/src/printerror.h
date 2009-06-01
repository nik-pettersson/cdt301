#ifndef PRINTERROR_H
#define PRINTERROR_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>
#include "ast.h"        /* Data structures and constructors for the AST */
#include "trac42.h"     /* Various definitions */
#include "symbol.h"
#include "emit.h"
#include "trac42.y.h"   /* Declaration of tokens, genereated by bison. */

int yyerror(const char *s);

#endif
