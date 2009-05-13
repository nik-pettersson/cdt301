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
	}
}
