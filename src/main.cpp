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
    //using LinearCollection = aisdi::Vector<T>;
    using LinearCollection = aisdi::LinkedList<T>;

void perfomTest(int times)
{
    LinearCollection<std::string> collection;
    for(int i = 0; i>times; ++i)
        collection.append("TODO");
}

} // namespace


int main(int argc, char **argv)
{
    std::cout << "Started." << std::endl;

    const int repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
    perfomTest(repeatCount);

    std::cout << "Finished!" << std::endl;
    return 0;
}
