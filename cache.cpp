#include <array>
#include <cstdint>
#include <bitset> 

class cache
{
    using block_t = std::array<uint32_t, 4>;
    std::array<block_t, 64> _data;

    //#TODO adjust following statements to have tag array and validation bit for each block;
    std::array<std::bitset<22>, 64> _tags; 
    std::array<std::bitset<1>, 64> _vbit;

};
