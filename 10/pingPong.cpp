#include <iostream>
#include <thread>
#include <atomic>

const size_t maxPP = 500000;
std::atomic<bool> check (true);

void ping ()
{
	size_t i = 0;
	while ( i < maxPP)
		while (check)
		{
			std::cout<<"ping"<<std::endl;
			i++;
			check = false;
		}
}

void pong ()
{	
	size_t i = 0;
	while ( i < maxPP)
		while (!check)
		{
			std::cout<<"pong"<<std::endl;
			i++;
			check = true;
		}
}

int main ()
{
	std::thread t1(ping);
	std::thread t2(pong);
	t1.join();
	t2.join();
	return 0;
}
