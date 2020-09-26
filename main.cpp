#include <iostream>
#include <chrono>
#include <thread>
#include <H5Cpp.h>
#include <tbb/tick_count.h>

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

    return 0;
}