#include "offset.h"

t_symtab *current;

void offsetType(t_tree n){
	static int formalOffset = 0;
	static int localOffset = 0;
	IOffset *io = (IOffset *)malloc(sizeof(IOffset));
	if(n != NULL){
		switch(n->Kind){
			case kProgram:
				offsetType(n->Node.Program.Functions);
				break;
			case kFunction:
				formalOffset = 2;
				localOffset = 0;
				current = n->Node.Function.Symbols;
				offsetType(n->Node.Function.Variables);
				//  retValueOffset = formalOffset
				symtab_add(current , "10Return");
				io->Offset = formalOffset;
				io->Type = n->Node.Function.Type;
				symtab_set(current, "10Return", io);
				offsetType(n->Node.Function.Next);
				break;
			case kVariable:
				offsetType(n->Node.Variable.Next);
				switch(n->Node.Variable.VarKind){
					case kFormal:
						io->Offset = formalOffset;
						io->Type = n->Node.Variable.Type;
						symtab_set(current, n->Node.Variable.Name, io);
						switch(n->Node.Variable.Type){
							case INT:
								//Set offset
								formalOffset += 1;
								break;
							case BOOL:
								//set offset
								formalOffset += 1;
								break;
							case STRING:
								//set offset
								formalOffset += 100;
								break;
							case VOID:
							case ERROR:
								break;
						}
						break;
					case kLocal:
						switch(n->Node.Variable.Type){
							case INT:
								localOffset -= 1;
								//Set offset
								break;
							case BOOL:
								localOffset -= 1;
								//set offset
								break;
							case STRING:
								localOffset -= 100;
								//Set offset
								break;
							case VOID:
								break;
							case ERROR:
								break;
						}
						io->Offset = localOffset;
						io->Type = n->Node.Variable.Type;
						symtab_set(current, n->Node.Variable.Name, io);
						break;
				}
				printf("Variable := %s\n", n->Node.Variable.Name);
				break;
			default:
				break;
		}
	}
}
