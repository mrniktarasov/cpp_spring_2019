#include <iostream>
#include <cstdlib>
#include "numbers.dat"

int search ( int soughtFor );
int primeCount ( int start, int end );
int isPrime ( const int pr );


int main ( int argc, char* argv[] ) {
	int result;
	int start;
	int end;
	
	if( argc >= 2 && ( ( argc - 1 ) % 2 ) == 0 )
	{
		for ( int i = 1; i < argc; i++ ) 
		{
			start = search ( std::atoi(argv[i]) );
			end = search ( std::atoi(argv[++i]) );
			if (start == - 1 || end == - 1)
				return - 1;
			result = primeCount ( start, end );
			std::cout << result << std::endl;
		} 
		return 0;
	} else 
		return - 1;
}

int search ( int soughtFor ) {
	int beg = 0;
	int end = Size;
	int middle;
	
	while ( beg <= end )
	{
		middle = beg + ( end - beg ) / 2;
		if ( soughtFor < Data[middle] )
			end = middle - 1;
		else if ( soughtFor > Data[middle] )	
			beg = middle + 1;
		else
			return middle;
	}
	return - 1;
}

int primeCount ( int start, int end ) {
	int counter = 0;
	int temp;
	
	for ( int i = start; i <= end; i++ )
	{
		if ( isPrime ( Data[i]) )
		{
			counter++;
			temp = i;
			i++;
			while ( Data[temp] == Data[i] )
			{
				counter++;
				i++;
			}
			i--;		
		}
	}
	return counter;	
}

int isPrime ( const int pr ) {
	if ( pr == 1 )
		return 0;
	for( int i = 2; i*i <= pr; i++ )
		if (pr % i == 0)
			return 0;	
	return	1;	
}

