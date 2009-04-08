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

int matchString(char * buf, int pos, char * target){
	FILE * fp = globalFile(SCAN_GET, "");
	while(1){
	
	}
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

char * matchId(char * arg){
	return "nope";
}

void getNextToken (int *token, int *value){
	static int lineno = 0;
	int match = FALSE;
	int flag = FALSE;
	char c;
	char * buf;
	FILE * fp = globalFile(SCAN_GET, "");
	printf("\n%d: ", lineno);
	while(!match){
		c = readNext(fp);
		if(c == '\n'){
			lineno++;
			printf("\n%d: ", lineno);
		}
		else if(c == '\t' || c == ' ');
			//ignore white spaces
		else if(c == '\0'){
			printf("EOF...");
			//*token = END; 
			match = TRUE;
		}
		else{
			switch(c){
				case '(':
					printf(" leftparanthesis ");
					break;
				case ')':
					printf(" rightparanthesis ");
					break;
				case ',':
					printf(" comma ");
					break;
				case '{':
					printf(" leftbrace ");
					break;
				case '}':
					printf(" rightbrace ");
					break;
				case ';':
					printf(" semicolon ");
					break;
				case '=':
					c = readNext(fp);
					if(c == '='){
						printf(" relationop(equal) ");
					}
					else{
						ungetc(c, fp);
						printf(" assignop ");
					}
					break;
				case '+':
					printf(" plusop ");
					break;
				case '-':
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
						printf(" divop ");
						ungetc(c, fp);
						flag = FALSE;
					}
					break;
				case '*':
					printf(" mulop ");
					break;
				case '!':
					c = readNext(fp);
					if( c != '='){
						ungetc(c, fp);
						printf(" notop ");
					}
					else
						printf(" relationop(not equal) ");
					break;
				case '>':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else
						printf(" relationop(bigger)");
					break;
				case '<':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else
						printf(" relationop(smaller) ");
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
					printf("num(%d)", atoi(buf));
					break;
				default:
					if(isAlpha(c)){
						switch(c){
							case 'r':
								//matchString("return", buf);
								break;
							case 'i':
								break;
							case 'w':
								break;
							case 'e':
								break;
							case 'v':
								break;
							default:
								while(isAlpha(c)){
									c = readNext(fp);
									if(!isAlpha(c))
										ungetc(c, fp);
								}
								printf(" id ");
								break;
						}
					}
					else
						printf(" not a token%c",c);
					break;
			}
		}
	}
	return;
}
