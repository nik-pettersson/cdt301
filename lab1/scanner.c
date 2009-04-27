/* Text: Lexical analyser 
 * Author: Niklas Pettersson, Lars Cederholm
 * Date: Tue Apr 7
 * SEALOFAPPROVAL: 2009-04-21-14:00
 */
#include "scanner.h"

	static int lineno = 1;
/**
 * Check if the char is a letter
 */
int isAlpha(char arg){
	if((arg > 64 && arg < 91) || (arg > 96 && arg < 123))
		return TRUE;
	else
		return FALSE;
}
/*
 * Check if the char is a numeric value
 */
int isNumeric(char arg){
	if(arg > 47 && arg < 58)
		return TRUE;
	else
		return FALSE;
}

/*
 * Handler for a file pointer
 */
FILE * globalFile(int cmd, char * file){
	static FILE * fp;
	switch (cmd){
		case SCAN_INIT:
			fp = fopen(file, "r");
			if(fp == NULL)
				fprintf(stderr, "Can't open %s\n", file);
			return fp;
		case SCAN_GET:
			return fp;
	}
	return NULL;
}
/*
 * Read next from file
 */
char readNext(FILE *fp){
	char ret;
	if(fread(&ret, 1, 1, fp));
	else
		ret = '\004';
	return ret;
}

/*
 * Match an Integer.
 */
char * matchNum(char c){
	char * buf = (char *)malloc(sizeof(char)*20);
	FILE * fp = globalFile(SCAN_GET, "");
	char tmp;
	int pos = 0;
	buf[pos] = c;
	pos++;
	while(1){
		tmp = readNext(fp);
		if(isNumeric(tmp)){
			buf[pos] = tmp;
			pos++;
		}
		else{
			ungetc(tmp, fp);
			buf[pos] = '\0';
			return buf;
		}
	}
}

int matchId(char * target, char * arg, int * ppos){
	int pos = *ppos;
	char c = arg[pos];
	FILE * fp = globalFile(SCAN_GET, "");
	for(;pos < (strlen(target));pos++, c = readNext(fp)){
		if(c == target[pos])
			arg[pos] = c;
		else{
			ungetc(c,fp);
			pos--;
			*ppos = pos;
			return 0;
		}
	}
	ungetc(c,fp);
	*ppos = pos;
	return 1;
}

void IdMatch(char * buf, int * ppos){
	FILE * fp = globalFile(SCAN_GET, "");
	int pos = *ppos;
	char c;// = readNext(fp);
	do{	
	pos++;
		c = readNext(fp);
		buf[pos] = c;
	}while(isAlpha(c) || isNumeric(c));
	buf[pos] = ' ';
	pos--;
	ungetc(c, fp);
	*ppos = pos;
}

int getLineNo(void){
	return lineno;
}

void getNextToken (int *token, int *value, char * str){
	int pos = 0;
	int match = FALSE;
	int flag = FALSE;
	char c;
	char * buf = (char *)malloc(sizeof(char)*64);
	FILE * fp = globalFile(SCAN_GET, "");
	
	while(!match){
		c = readNext(fp);
		if(c == '\n'){
			lineno++;
		}
		else if(c == '\t' || c == ' '||  c == '\0');
		//ignore white spaces
		else if(c == '\004'){
			*token = END; 
			match = TRUE;
		}
		else{
			switch(c){
				case '(':
					*token = LEFTPARENTHESIS;
					match = TRUE;
					break;
				case ')':
					*token = RIGHTPARENTHESIS;
					match = TRUE;
					break;
				case ',':
					*token = COMMA;
					match = TRUE;
					break;
				case '{':
					*token = LEFTBRACE;
					match = TRUE;
					break;
				case '}':
					*token = RIGHTBRACE;
					match = TRUE;
					break;
				case ';':
					*token = SEMICOLON;
					match = TRUE;
					break;
				case '=':
					*token = RELATIONOP;
					c = readNext(fp);
					if(c == '='){
						*value = EQUAL;
					}
					else{
						ungetc(c, fp);
						*token = ASSIGNOP;
					}
					match = TRUE;
					break;
				case '+':
					*token = PLUSOP;
					match = TRUE;
					break;
				case '-':
					*token = MINUSOP;
					match = TRUE;
					break;
				case '/':
					c = readNext(fp);
					if(c == '*'){
						while(c != '/' && !flag){
							flag = FALSE;
							c = readNext(fp);
							if(c == '\n'){
								lineno++;
							}
							else if(c == '*'){
								flag = TRUE;
								c = readNext(fp);
							}
						}
						flag = FALSE;
						match = FALSE;
						break;
					}
					else{
						*token = DIVOP;
						ungetc(c, fp);
						flag = FALSE;
					}
					match = TRUE;
					break;
				case '*':
					*token = MULTOP;
					match = TRUE;
					break;
				case '!':
					c = readNext(fp);
					if( c != '='){
						*token = NOTOP;
						ungetc(c, fp);
					}
					else{
						*token = RELATIONOP;
						*value = NOTEQUAL;
					}
					match = TRUE;
					break;
				case '>':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else{
						*token = RELATIONOP;
						*value = BIGGER;
					}
					match = TRUE;
					break;
				case '<':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else{
						*token = RELATIONOP;
						*value = SMALLER;
					}
					match = TRUE;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					buf = matchNum(c);
					*token = NUM;
					*value = atoi(buf);
					match = TRUE;
					break;
				default:
					if(isAlpha(c)){
						pos = 0;
						switch(c){
							case 'r':
								buf[pos] = c;
								if(matchId("return", buf, &pos)){
									*token = RETURN;
								}
								else if(matchId("read", buf, &pos))
									*token = READ;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strncpy(str, buf, 64);
								}
								break;
							case 'i':
								buf[pos] = c;
								if(matchId("if", buf, &pos))
									*token = IF;
								else if(matchId("int", buf, &pos))
									*token = INT;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strncpy(str, buf, 64);
								}
								break;
							case 'w':
								buf[0] = c;
								if(matchId("while", buf, &pos))
									*token = WHILE;
								else if(matchId("write", buf, &pos))
									*token = WRITE;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strncpy(str, buf, 64);
								}
								break;
							case 'e':
								buf[0] = c;
								if(matchId("else", buf, &pos)){
									*token = ELSE;
								}
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strncpy(str, buf, 64);
								}
								break;
							case 'v':
								buf[0] = c;
								if(matchId("void", buf, &pos))
									*token = VOID;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strncpy(str, buf, 64);
								}
								break;
							default:
								buf[0] = c;
								IdMatch(buf, &pos);
								*token = ID;
								strncpy(str, buf, 64);
								break;
						}
						//strncpy(str,buf,64);
						match = TRUE;
						c = readNext(fp);
						ungetc(c, fp);
					}
					else{
						*token = ERROR;
						match = TRUE;
					}
			}
		}
	}
	// DEBUGGING PRINT
	printToken(*token);
	free(buf);
	printf("POS %d\n", pos);
	str[pos+1] = '\0';
	return;
}

void printToken(int tok) {
	switch(tok) {
		case RETURN:
			printf("RETURN\n");
			break;
		case IF:
			printf("IF\n");
			break;
		case WHILE:
			printf("WHILE\n");
			break;
		case WRITE:
			printf("WRITE\n");
			break;
		case READ:
			printf("READ\n");
			break;
		case ELSE:
			printf("ELSE\n");
			break;
		case VOID:
			printf("VOID\n");
			break;
		case INT:
			printf("INT\n");
			break;
		case LEFTBRACE:
			printf("LEFTBRACE\n");
			break;
		case RIGHTBRACE:
			printf("RIGHTBRACE\n");
			break;
		case LEFTPARENTHESIS:
			printf("LEFTPARENTHESIS\n");
			break;
		case RIGHTPARENTHESIS:
			printf("RIGHTPARENTHESIS\n");
			break;
		case SEMICOLON:
			printf("SEMICOLON\n");
			break;
		case COMMA:
			printf("COMMA\n");
			break;
		case ASSIGNOP:
			printf("ASSIGNOP\n");
			break;
		case PLUSOP:
			printf("PLUSOP\n");
			break;
		case MINUSOP:
			printf("MINUSOP\n");
			break;
		case MULTOP:
			printf("MULTOP\n");
			break;
		case DIVOP:
			printf("DIVOP\n");
			break;
		case NOTOP:
			printf("NOTOP\n");
			break;
		case RELATIONOP:
			printf("RElATIONOP\n");
			break;
		case ID:
			printf("ID\n");
			break;
		case NUM:
			printf("NUM\n");
			break;
		case ERROR:
			printf("ERROR\n");
			break;
		case END:
			printf("\n***END***");
			break;
	}
}
