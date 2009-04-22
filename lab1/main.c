#include "scanner.h"
#include "printer.h"

int main(void){
	int x, y;
	char * str = (char *)malloc(sizeof(char)*64);
	globalFile(SCAN_INIT, "examples/fac.t42");
	for(getNextToken(&x, &y, str); x != END; getNextToken(&x, &y, str)){
		printer(x, y, str);
		printf("\n%d:", getLineNo());
		
		x = 0;
		y = 0;
		strncpy(str, "", 64);
	}
	return 4;
}
