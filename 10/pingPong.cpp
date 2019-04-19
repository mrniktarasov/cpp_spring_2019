#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable condOne;
std::condition_variable condTwo;

void ping ()
{
	std::unique_lock<std::mutex> lock(mutex);
	for ( size_t i = 0; i < 500000; i++ )
	{	
		std::cout<<"ping"<<std::endl;
		condTwo.notify_one();
		condOne.wait(lock);
	}
	condTwo.notify_one();
}

void pong ()
{
	std::unique_lock<std::mutex> lock(mutex);
	for ( size_t i = 0; i < 500000; i++ )
	{
		std::cout<<"pong"<<std::endl;
		condOne.notify_one();
		condTwo.wait(lock);
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
