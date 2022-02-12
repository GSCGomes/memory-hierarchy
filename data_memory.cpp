#include <array>
#include <iostream>
#include <cstdint>
#include <bitset>

#define DEBUG_MEM(code) if (1) { code; }

class data_memory
{
    using block_t = std::array<std::bitset<32>, 4>;

  public:
    data_memory(){
        for (int i = 0; i < 1024; i++){
            _data[i] = 0x0;
        }
    }

    block_t read_block(int word_addr) {  // Method/function defined inside the class

        block_t block;
        int block_start = word_addr - word_addr % 4;

        for(int i = block_start, j = 0; i < block_start + 4; ++i, ++j){
            block[j] = _data[i];
        }

        DEBUG_MEM(std::cout << "DEBUG_MEM\t" << "read_block "
                            << "word_addr= " << word_addr
                            << ", block_start= " << block_start
                            << ", block= ")
        DEBUG_MEM(for (int i = 0; i < 4; ++i) std::cout << block[i] << " ")
        DEBUG_MEM(std::cout << std::endl)


        return block;
    }

    void write_word(int addr, const std::bitset<32>& write_data) {  // Method/function defined inside the class
        _data[addr] = write_data;

        DEBUG_MEM(std::cout << "DEBUG_MEM\t" << "write_word "
                            << "addr= " << addr
                            << ", write_data= " << write_data
                            << std::endl;)
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

  private:
    std::array<std::bitset<32>, 1024> _data;
};

