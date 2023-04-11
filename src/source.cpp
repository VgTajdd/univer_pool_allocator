#include "pkg/pkg.h"

int main( int, char** )
{
	pkg::printHelloWorld();
	pkg::printNumber( pkg::getFibonacci( 5 ) );
	return 0;
}
