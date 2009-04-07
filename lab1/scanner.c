/* Text: Lexical analyser 
 * Author: Niklas Pettersson, Lars Cederholm
 * Date: Tue Apr 7
 */
#include "scanner.h"

int isAlpha(char arg){
	if((arg > 64 && arg < 91) || (arg > 96 && arg < 123))
		return TRUE;
	else
		return FALSE;
}

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
		switch(tmp){
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
				buf[pos] = tmp;
				pos++;
				break;
			default:
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
				case '[':
					printf(" leftbrace ");
					break;
				case ']':
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
					printf(" divop ");
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
					//TODO FIX
				case '<':
					c = readNext(fp);
					if( c != '=')
						ungetc(c, fp);
					else
						printf(" relationop(bigger smaller ");
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
					}
					else
						printf("%c",c);
			}
		}
		if(c == '\0')
			match = TRUE;
	}
	return;
}
