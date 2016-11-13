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
    using LinearCollection = aisdi::Vector<T>;
//    using LinearCollection = aisdi::LinkedList<T>;

//    void perfomTest()
//    {
//        LinearCollection<std::string> collection;
//        collection.append("TODO");
//    }

} // namespace


int main()
{
    std::cout << "Works!" << std::endl;


//  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
//  for (std::size_t i = 0; i < repeatCount; ++i)
//    perfomTest();
    return 0;
}
