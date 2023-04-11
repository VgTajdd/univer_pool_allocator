#include "pkg/pkg.h"

#include <iostream>

void pkg::printHelloWorld()
{
	std::cout << "Hello World!" << std::endl;
}

void pkg::printNumber( int number )
{
	std::cout << number << std::endl;
}

int pkg::getFibonacci( int n )
{
	if ( n == 0 )
	{
		return 0;
	}
	else if ( n == 1 )
	{
		return 1;
	}
	else
	{
		return pkg::getFibonacci( n - 1 ) + pkg::getFibonacci( n - 2 );
	}
}
