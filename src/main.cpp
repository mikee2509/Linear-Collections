#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include "Vector.h"
#include "LinkedList.h"
#include <vector>
#include <math.h>
namespace
{

template <typename T>
    //using LinearCollection = aisdi::LinkedList<T>;
    using LinearCollection = aisdi::Vector<T>;

void perfomTest(long long times)
{
    LinearCollection<std::string> collection;
    for(long long i = 0; i<times; ++i)
        collection.append("TODO");
    for(long long i = 0; i<times; ++i)
        collection.prepend("TODO");
}

} // namespace


int main(int argc, char **argv)
{
    //std::cout << "LinearCollection = LinkedList" << std::endl;
    std::cout << "LinearCollection = Vector" << std::endl;

    const long long repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
    perfomTest(repeatCount);

    std::cout << "Finished!" << std::endl;
    return 0;
}
