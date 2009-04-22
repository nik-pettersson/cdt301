/* Text: Lexical analyser 
 * Author: Niklas Pettersson, Lars Cederholm
 * Date: Tue Apr 7
 */
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define RETURN (1)
#define IF	(2)
#define WHILE (3)
#define WRITE (4)
#define READ (5)
#define ELSE (6)
#define VOID (7)
#define INT (8)
#define LEFTBRACE (9)
#define RIGHTBRACE (10)
#define LEFTPARENTHESIS (11)
#define RIGHTPARENTHESIS (12)
#define SEMICOLON (13)
#define COMMA (14)
#define ASSIGNOP (15)
#define PLUSOP (16)
#define MINUSOP (17)
#define MULTOP (18)
#define DIVOP (19)
#define NOTOP (20)
#define RELATIONOP (21) //är någon av == <= >= != 
	#define EQUAL (1)
	#define NOTEQUAL (2)
	#define BIGGER	(3)
	#define SMALLER	(4)
#define ID (22) // där lexemet som ID representerar är: bokstav(bokstav U siffra)*
#define NUM (23) //där lexemet som NUM representerar är: siffra+
#define TRUE 	(1)
#define FALSE (0)
#define ERROR (-1)
#define END (42)

#define SCAN_INIT	(0)
#define SCAN_GET	(1)


FILE * globalFile(int cmd, char * file);
int getLineNo(void);
void getNextToken (int *token, int *value, char * str);
void printToken(int tok);
#endif
