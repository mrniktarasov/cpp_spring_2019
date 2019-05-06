#pragma ones
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "mergesort.cpp"
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>

constexpr size_t Mb = 1024*1024;
constexpr size_t MAXSIZE = (8*Mb - sizeof(std::thread)*2);
using Number = uint64_t;

int main()
{

	size_t size = MAXSIZE/2; // size of array 
	const char* dataFileName = "data.txt";
	std::ifstream dataFile (dataFileName, std::ios::binary);
	Sort sort;
	while (!dataFile.eof())
	{
		Number* data = new Number [size];
		dataFile.read(reinterpret_cast<char*>(data), size/sizeof(char));
		sort.merge_sort_start(data, size, 0, size - 1);
		delete [] data;
	}
	mergeFiles(sort.getFilesNames);
}
