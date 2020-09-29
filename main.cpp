#include <iostream>
#include <chrono>
#include <thread>
#include <tbb/tick_count.h>
#include <boost/filesystem.hpp>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#include <H5Cpp.h>
#pragma warning(pop)
#endif

int main(int, char**)
{
    tbb::tick_count startTime = tbb::tick_count::now();
    for (int n=0; n<100; ++n)
    {
        std::cout << "Hello, world!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10) );
    }
    
    tbb::tick_count endTime = tbb::tick_count::now();
    std::cout << (endTime-startTime).seconds()*1000<<"ms"<<std::endl;
    std::cout << boost::filesystem::current_path() << std::endl;

    std::cin.clear();
    std::cin.get();

    return 0;
}