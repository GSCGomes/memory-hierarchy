#include <iostream>
#include "data_memory.cpp"
#include "cache.cpp"

int main()
{
    data_memory D;
    cache C;
    std::cout << "size of data memory: " << sizeof(D) << " bytes"
              << "\nsize of cache: " << sizeof(C) << " bytes"
              << std::endl;
    return 0;
}
