#include <array>
#include <iostream>
#include <optional>
#include <cstdint>
#include <bitset>

class cache
{
    using word_t = std::bitset<32>;

    using block_t = std::array<std::bitset<32>, 4>;
    std::array<block_t, 64> _data;

    //each tag or validity bit represents a block
    std::array<std::bitset<22>, 64> _tags;
    std::array<std::bitset<1>, 64> _vbit;
    std::array<std::bitset<1>, 64> _dbit;

    public:
    void init(){
         for (int i = 0; i < 64; i++){
                for (int j = 0; j < 4; j++){
                    _data[i][j] = 0x0;
                }
                _tags[i] = 0x0;
                _vbit[i] = 0x0;
                _dbit[i] = 0x0;
            }
        }

    public:
    std::optional<word_t> read(int block_number, const std::bitset<22>& tag, int word_offset) {
      if((_vbit[block_number] == 1) && (_tags[block_number] == tag)){
          return _data[block_number][word_offset];
      }
      return {};
    }

    int get_dbit(int block_number) {  // Method/function defined inside the class
        return _dbit[block_number].to_ulong();
    }

    public:
    void write_from_memory(int block_number, std::bitset<22> tag, std::array<std::bitset<32>, 4> _block_data) {  // Method/function defined inside the class
        _tags[block_number] = tag;
        _vbit[block_number] = 1;
        _data[block_number] = _block_data;
    }

    public:
    void write(int block_number, int word_offset, std::bitset<22> tag, std::bitset<32> write_data) {  // Method/function defined inside the class
        _tags[block_number] = tag;
        _vbit[block_number] = 1;
        _dbit[block_number] = 1;
        _data[block_number][word_offset] = write_data;
    }

    void dump() {
        auto printSeparator = [](const char* str) {
            const int n = 90;
            for (int i = 0; i < n; ++i) std::cout << "-";
            std::cout << str;
            for (int i = 0; i < n; ++i) std::cout << "-";
            std::cout << "\n\n";
        };

        printSeparator("cache start");

        std::cout << "\tV\tD\tT\t\t\t\tBLOCK\n";
        for (int i = 0; i < 64; ++i){
            std::cout << i << ":\t";
            std::cout << _vbit[i] << "\t"
                      << _dbit[i] << "\t"
                      << _tags[i] << "\t\t";
            for (int j = 0; j < 4; ++j)
                std::cout << _data[i][j] << " ";
            std::cout << std::endl;

        }

        printSeparator("cache end");
    }


};

