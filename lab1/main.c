#include "scanner.h"
#include "printer.h"
#include "parser.h"

int main(void){
	int x, y;
	char * str = (char *)malloc(sizeof(char)*64);
	globalFile(SCAN_INIT, "examples/fac.t42");
	printf("And the result is.............\n%d!!!!!!!!!!\n", Prog("program.sc"));
	return 0;
	for(getNextToken(&x, &y, str); x != END; getNextToken(&x, &y, str)){
		printer(x, y, str);
		printf("\n%d:", getLineNo());
		
		x = 0;
		y = 0;
		strncpy(str, "", 64);
	}
	return 4;
}
