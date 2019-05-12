#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <experimental/filesystem>
#include <cstdio>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <thread>

namespace fs = std::experimental::filesystem;

constexpr std::size_t kB = 1024;
constexpr std::size_t MB = 1024 * kB;
constexpr std::size_t memSize = 8*MB - sizeof(std::vector);//8*MB;
constexpr std::size_t Len = memSize/sizeof(std::uint64_t);
constexpr std::size_t N = 3*Len+3;

template<class Iter>
void merge(Iter beg, Iter mid, Iter end) 
{ 
    auto p = mid-1;
    if (*p <= *mid) 
    { 
        return; 
    } 
   
    while (beg <= p && mid < end) 
    { 
        if (*beg <= *mid) 
        { 
            beg++; 
        } 
        else 
        { 
            auto value = *mid; 
            auto it = mid;  
            while (it != beg) 
            { 
                *it = *(it - 1); 
                it--; 
            } 
            *beg = value; 
            beg++; 
            mid++; 
            p++; 
        } 
    } 
} 

template<class Iter>
void csort(Iter first, Iter last)
{
    if (last - first > 1)
    {
        Iter middle = first + (last - first) / 2;
        std::thread t1(std::sort,first,middle);
        std::thread t2(std::sort,middle, last);
        t1.join();
        t2.join();
        merge(first, middle, last);
    }
}



int main(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cout<<"Not enough command line arguments\n";
        return -1;
    }

    std::ofstream out(argv[1],std::ios::binary);
	if (!out)
    {
        std::cerr << "Can't open " << fileName << '\n';
        exit(1);
    }
    std::vector<std::uint64_t> vec(N);

    auto start = std::chrono::high_resolution_clock::now();
    auto fsize = fs::file_size(fs::path{argv[1]});//размер файла
    out.open(argv[2],std::ios::binary);
    if (fsize > memSize)
    {
        std::string f1(argv[1]),f2("td2");

        while(true)
        {
            fsize = fs::file_size(fs::path{f1});
            std::ifstream is(f1,std::ios::binary);
            std::remove(f2.c_str());
            std::ofstream os(f2,std::ios::binary|std::ios::app);
			if (!os)
			{
				std::cerr << "Can't open " << fileName << '\n';
				exit(1);
			}
            std::size_t h = Len/2;
            std::size_t m = h;
            if (Len%2 != 0)
            {
                m = Len - h;
            }
            std::vector<std::uint64_t> data(Len);
            is.read((char*)data.data()[0], m*sizeof(std::uint64_t));
            fsize -= m*sizeof(std::uint64_t);
            csort(data.begin(),data.begin()+m);

            while(true)
            {
                if(fsize > memSize/2)
                {
                    is.read((char*)data.data()[m], h*sizeof(std::uint64_t));
                    csort(data.begin()+m,data.end());

                    merge(data.begin(), data.begin()+m, data.end());
                    os.write((char*)data.data()[m], h*sizeof(std::uint64_t));
                    fsize -= h*sizeof(std::uint64_t);
                }
                else
                {
                    if(fsize != 0)
                    {
                        is.read((char*)data.data()[m], fsize);
                        data.resize(m+fsize/sizeof(std::uint64_t));

                        csort(data.begin()+m,data.end());
                        merge(data.begin(), data.begin()+m, data.end());
                        os.write((char*)data.data()[m], fsize);
                    }
                    out.write((char*)data.data()[0], m*sizeof(std::uint64_t));
                    break;
                }
            }

            os.close();
            is.close();
            auto csize = fs::file_size(fs::path{f2});

            if(csize <= memSize)
            {
                std::ifstream in(f2,std::ios::binary);
                data.resize(csize/sizeof(std::uint64_t));
                in.read((char*)data.data()[0], csize);
                csort(data.begin(),data.end());
                out.write((char*)data.data()[0], csize);
                out.close();
                in.close();
                std::remove("td2");
                std::remove("td3");

                break;
            }
            else
            {
                f1 = f2;
                if(f1 == "td2")
                {
                    f2 = "td3";
                }
                else
                {
                    f2 = "td2";
                }
            }
        }
    }
    else
    {
        std::ifstream in(argv[1],std::ios::binary);
        in.read((char*)vec.data()[0], fsize);
        vec.resize(fsize/sizeof(std::uint64_t));
        csort(vec.begin(),vec.end());
        out.write((char*)vec.data()[0], fsize);
        out.close();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Task takes "<< diff.count() << " s\n";
}
