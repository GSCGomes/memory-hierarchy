#include <array>
#include <cstdint>

class cache
{
    using block_t = std::array<uint32_t, 4>;
    std::array<block_t, 64> _data;
};
