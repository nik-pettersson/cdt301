/* Text: Lexical analyser 
 * Author: Niklas Pettersson, Lars Cederholm
 * Date: Tue Apr 7
 */
#include "scanner.h"

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
	fread(&ret, 1, 1, fp);
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
	FILE * fp = globalFile(SCAN_GET, "");
	for(;pos < strlen(target);pos++, arg[pos] = readNext(fp)){
		if(arg[pos] == target[pos]);
		else{
			ungetc(arg[pos],fp);
			arg[pos] = ' ';
			*ppos = pos;
			return 0;
		}
	}
	*ppos = pos;
	return 1;
}

void IdMatch(char * buf, int * ppos){
	FILE * fp = globalFile(SCAN_GET, "");
	int pos = *ppos;
	char c = readNext(fp);
	while(isAlpha(c) || isNumeric(c)){
		pos++;
		c = readNext(fp);
		buf[pos] = c;
	}
	ungetc(c, fp);
	*ppos = pos;
}

void getNextToken (int *token, int *value, char * str){
	static int lineno = 0;
	int pos;
	int match = FALSE;
	int flag = FALSE;
	char c;
	char * buf = (char *)malloc(sizeof(char)*64);
	FILE * fp = globalFile(SCAN_GET, "");
	printf("\n%d: ", lineno);
		c = readNext(fp);
		if(c == '\n'){
			lineno++;
			printf("\n%d: ", lineno);
		}
		else if(c == '\t' || c == ' ');
		//ignore white spaces
		else if(c == '\0'){
			//printf("EOF...");
			*token = END; 
			match = TRUE;
		}
		else{
			switch(c){
				case '(':
					*token = LEFTPARENTHESIS;
					printf(" leftparanthesis ");
					break;
				case ')':
					*token = RIGHTPARENTHESIS;
					printf(" rightparanthesis ");
					break;
				case ',':
					*token = COMMA;
					printf(" comma ");
					break;
				case '{':
					*token = LEFTBRACE;
					printf(" leftbrace ");
					break;
				case '}':
					*token = RIGHTBRACE;
					printf(" rightbrace ");
					break;
				case ';':
					*token = SEMICOLON;
					printf(" semicolon ");
					break;
				case '=':
					*token = RELATIONOP;
					c = readNext(fp);
					if(c == '='){
						*value = EQUAL;
						printf(" relationiop(equal) ");
					}
					else{
						ungetc(c, fp);
						*token = ASSIGNOP;
						printf(" assignop ");
					}
					break;
				case '+':
					*token = PLUSOP;
					printf(" plusop ");
					break;
				case '-':
					*token = MINUSOP;
					printf(" minusop ");
					break;
				case '/':
					c = readNext(fp);
					if(c == '*'){
						while(c != '/' && !flag){
							flag = FALSE;
							c = readNext(fp);
							if(c == '\n'){
								lineno++;
								printf("\n%d:", lineno);
							}
							else if(c == '*'){
								flag = TRUE;
								c = readNext(fp);
							}
						}
						printf(" COMMENT ");
						//ungetc(c, fp);
						flag = FALSE;
						break;
					}
					else{
						*token = DIVOP;
						printf(" divop ");
						ungetc(c, fp);
						flag = FALSE;
					}
					break;
				case '*':
					*token = MULTOP;
					printf(" mulop ");
					break;
				case '!':
					c = readNext(fp);
					if( c != '='){
						*token = NOTOP;
						ungetc(c, fp);
						printf(" notop ");
					}
					else{
						*token = RELATIONOP;
						*value = NOTEQUAL;
						printf(" relationop(not equal) ");
					}
					break;
				case '>':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else{
						*token = BIGGER;
						printf(" relationop(bigger)");
					}
					break;
				case '<':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else{
						*token = SMALLER;
						printf(" relationop(smaller) ");
					}
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
					printf("num(%d)", atoi(buf));
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
									strcpy(str, buf);
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
									strcpy(str, buf);
								}
								break;
							case 'w':
								buf[0] = c;
								if(matchId("while", buf, 0))
									*token = WHILE;
								else if(matchId("write", buf, 0))
									*token = WRITE;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strcpy(str, buf);
								}
								break;
							case 'e':
								buf[0] = c;
								if(matchId("else", buf, 0))
									*token = ELSE;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strcpy(str, buf);
								}
								break;
							case 'v':
								buf[0] = c;
								if(matchId("void", buf, 0))
									*token = VOID;
								else{
									IdMatch(buf, &pos);
									*token = ID;
									strcpy(str, buf);
								}
								break;
							default:
								buf[0] = c;
								IdMatch(buf, &pos);
								*token = ID;
								strcpy(str,buf);
						}
					}
					else
						*token = ERROR;
					break;
			}
		}
	return;
}
