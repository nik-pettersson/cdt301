#ifndef CODEGEN_H
#define CODEGEN_H

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
#include "offset.h"
#include "printerror.h"

eType codeType(t_tree n, tCode *c);
tCode * generateCode(t_tree root);

#endif
