#include "vartype.h"

char *varType(eType t) {
	switch (t) {
		case VOID:
			return "void";
		case BOOL:
			return "bool";
		case INT:
			return "int";
		case STRING:
			return "string";
		case ERROR:
			return "error";
	}
	return "unknown";
}
