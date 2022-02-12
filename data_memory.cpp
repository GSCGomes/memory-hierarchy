#include <array>
#include <iostream>
#include <cstdint>
#include <bitset>

class data_memory
{
    using block_t = std::array<std::bitset<32>, 4>;
    block_t _block_data;
    std::array<std::bitset<32>, 1024> _data;

    public:
    void init(){
         for (int i = 0; i < 1024; i++){
                _data[i] = 0x0;
            }
        }

    public:
    block_t read(int word_addr) {  // Method/function defined inside the class

        int block_start = word_addr - word_addr % 4;

        int j = 0;
        for(int i = block_start; i < block_start + 4; i++){
            _block_data[j] = _data[i];
            j++;
        }

        return _block_data;
    }

    public:
    void write(int addr, std::bitset<32> write_data) {  // Method/function defined inside the class
        _data[addr] = write_data;
    }

    void dump() {
        auto printSeparator = [](const char* str) {
            const int n = 20;
            for (int i = 0; i < n; ++i) std::cout << "-";
            std::cout << str;
            for (int i = 0; i < n; ++i) std::cout << "-";
            std::cout << "\n\n";
        };

        printSeparator("data memory start");

        // std::cout << "\tV\tD\tT\t\t\t\tBLOCK\n";
        for (int i = 0; i < 1024; ++i){
            std::cout << i << ":\t"
                      << _data[i];
            if (_data[i].to_ulong()) {
                std::cout << "\t<------";
            }
            std::cout << "\n";
        }

        printSeparator("data memory end");
    }

};

