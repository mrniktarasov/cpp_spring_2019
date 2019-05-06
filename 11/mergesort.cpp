
#include <iostream>
#include <pthread.h>
#include "mergesort.h"
#include <vector>
#include <mutex>
#include <atomic>

using Number = uint64_t;

void *merge_sort(void* p);
void merge(Number *numbers, size_t size, size_t low, size_t mid, size_t high);
void load(struct SortArgs *p, size_t *_numbers, size_t _size, size_t _low, size_t _high);


/* Holds array data to be passed size_to pthread_create */
struct SortArgs
{
	Number *numbers;
	size_t size;
	size_t low;
	size_t high;
};

class Sort 
{
/* Loads the parameters size_to the structure */
	void load(struct SortArgs *p, size_t *_numbers, size_t _size, size_t _low, size_t _high)
	{
		p->numbers = _numbers;
		p->size = _size;
		p->low = _low;
		p->high = _high;
	}

/* Recursive merging and sorting */
	void *merge_sort(void* p)
	{
		struct SortArgs *args = (struct SortArgs *) p;
		size_t mid;
		struct SortArgs left, right;

		if (args->low < args->high)
		{
			mid = (args->low + args->high) / 2;
			load(&left, args->numbers, args->size, args->low, mid);
			merge_sort((void *) &left);

			load(&right, args->numbers, args->size, mid + 1, args->high);
			merge_sort((void *) &right);

			merge(args->numbers, args->size, args->low, mid, args->high);
		}

	return NULL;
	}

/* Merges arrays together */
	void merge(Number *numbers, size_t size, size_t low, size_t mid, size_t high)
	{
		Number temp[size];
		size_t h, i, j, k;
		h = low;
		i = low;
		j = mid + 1;

	// traverse both parts of the array...
		while ((h <= mid) && (j <= high))
		{ 

		// if an element of left half of the array is 
		// less than element of right half of the array...
			if (numbers[h] <= numbers[j])
			{

		// store left-half element in the temp array
				temp[i] = numbers[h]; 

		// now shift the index of the array 
		// from which the element was copied to temp 
				h++;
			}

		// else, store the right half element in the temp array
		else
			{
				temp[i] = numbers[j];
			// now shift the index of the array from 
			// which the element was copied to temp 
				j++; 
			}
			i++;
		}

	// if we're done traversing the left half,
	// copy remaining elements from right half to temp
	if (h > mid)
	{
		for (k = j; k <= high; k++)
		{
			temp[i] = numbers[k];
			i++;
		}
	}

	// else, copy remaining elements from left half to temp
	else
	{
		for (k = h; k <= mid; k++) 
		{
		temp[i] = numbers[k];
		i++;
		}
	}
	saveToFile(temp, low, high);
  // recopy the values from temp to original array.
	}
	
	void saveToFile (Number* temp, size_t low, size_t high)
	{
		
		std::lock_guard <std::mutex> lock(resultMutex);
		const std::string fileName = std::to_string(count);
		count++;
		std::ofstream file(fileName);
		if (!file)
		{
			std::cerr << "Can't open " << fileName << '\n';
			exit(1);
		}
		
		files_.push_back(fileName);

		for (size_t k = low; k <= high; k++)
			file<<" "<<temp[k];
	}

/* Initial merging and sorting */
	public:
	std::vector<std::string> files_;
	std::mutex resultMutex;
	std::atomic<int> count = {0};
	
	void merge_sort_start(Number *numbers, size_t size, size_t low, size_t high)
	{
		size_t mid = (low + high) / 2;
		struct SortArgs left, right;

		pthread_t thread_left, thread_right;

		load(&left, numbers, size, low, mid);
		load(&right, numbers, size, mid + 1, high);

		pthread_create(&thread_left, NULL, &Sort::merge_sort, (void *) &left);
		pthread_create(&thread_right, NULL, &Sort::merge_sort, (void *) &right);

		pthread_join(thread_left, NULL);
		pthread_join(thread_right, NULL);

		merge(numbers, size, low, mid, high);
	}
	
	const decltype(files_) getFilesNames()
	{
		return files_;
	}
};


void mergeFiles (std::vector<std::string> &filesNames)
{
	size_t c = 0;
	const char* resultFileName = "result.txt";
	std::ofstream resultFile("result.txt");
	do
	{
		
		std::string fileName1 = filesNames[c];
		std::string fileName2 = filesNames[c+1];
		std::ifstream file1(fileName1);
		std::ifstream file2(fileName2);
		Number num1;
		Number num2;
		while (file1>>num1)
			while (file2>>num2)
			{
				if (num1 > num2)
				{
					resultFile<<" "<<num1<<" "<<num2;
				}
				else
				{
					resultFile<<" "<<num2<<" "<<num1;
				}
			}
		filesNames.push_back(fileName2);
		filesNames.erase(filesNames.begin()+1);
	} while(!filesNames.size() == 2);
}
