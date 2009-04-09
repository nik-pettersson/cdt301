#include "scanner.h"

int main(void){
	int x, y;
	globalFile(SCAN_INIT, "examples/fac.t42");
	getNextToken(&x, &y);
	return 4;
}
