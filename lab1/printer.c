/* Text: Function for printing an intermidiate language.
 * Authors: Niklas Pettersson, Lars Cederholm
 * Date: 21 April
 */

#include "scanner.h"
#include <stdio.h>

void printer(int token, int value, char * str){
	switch(token){
		case RETURN:
			break;
		case IF:
			break;
		case WHILE:
			break;
		case WRITE:
			break;
		case READ:
			break;
		case ELSE:
			break;
		case VOID:
			break;
		case INT:
			break;
		case LEFTBRACE:
		case RIGHTBRACE:
		case LEFTPARENTHESIS:
		case RIGHTPARENTHESIS:
		case SEMICOLON:
		case COMMA:
			break;
		case ASSIGNOP:
			printf("\n:=");
			break;
		case PLUSOP:
			printf("\n+");
			break;
		case MINUSOP:
			printf("\n-");
			break;
		case MULTOP:
			printf("\n*");
			break;
		case DIVOP:
			printf("\n/");
			break;
		case NOTOP:
			break;
		case RELATIONOP:
			switch(value){
				case EQUAL:
				case NOTEQUAL:
				case BIGGER:
				case SMALLER:
				default:
					break;
			}
		case ID:
			printf("\nlvalue %s", str);
			break;
		case NUM:
			printf("\npush %d", value);
			break;
		default:
			break;
	}
}
