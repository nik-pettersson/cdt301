#include "scanner.h"

int main(void){
	int x, y;
	char * str = (char *)malloc(sizeof(char)*64);
	globalFile(SCAN_INIT, "examples/test.t42");
	for(getNextToken(&x, &y, str); x != END; getNextToken(&x, &y, str)){
		printf("\ngetNextToken: %d %d %s", x, y , str);
		x = 0;
		y = 0;
		strncpy(str, "", 64);
	}
	return 4;
}
