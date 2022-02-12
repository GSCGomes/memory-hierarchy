#include <array>
#include <iostream>
#include <optional>
#include <cstdint>
#include <bitset>

#define DEBUG_CACHE(code) if (1) { code; }

class cache
{
    using word_t = std::bitset<32>;

    using block_t = std::array<std::bitset<32>, 4>;
    std::array<block_t, 64> _data;

    //each tag or validity bit represents a block
    std::array<std::bitset<22>, 64> _tags;
    std::array<bool, 64> _vbit;
    std::array<bool, 64> _dbit;

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
    std::optional<word_t> read_word(int block_number, const std::bitset<22>& tag, int word_offset) {
        DEBUG_CACHE(std::cout << "DEBUG_CACHE\t" << "read_word "
                              << "block_number= " << block_number
                              << ", tag= " << tag
                              << ", word_offset= " << word_offset)

        if(_vbit[block_number] && (_tags[block_number] == tag)){
            DEBUG_CACHE(std::cout << ", hit :D word= " << _data[block_number][word_offset] << std::endl)
            return _data[block_number][word_offset];
        }
        DEBUG_CACHE(std::cout << ", miss :(" << std::endl);
        return {};
    }

    bool is_dirty(int block_number) {  // Method/function defined inside the class
        DEBUG_CACHE(std::cout << "DEBUG_CACHE is_dirty " << _dbit[block_number] << std::endl)
        return _dbit[block_number];
    }

    void write_block(int block_number, const std::bitset<22>& tag, block_t data, bool dirty = 0) {
        DEBUG_CACHE(std::cout << "DEBUG_CACHE\t" << "write_block "
                              << "block_number= " << block_number
                              << ", tag= " << tag
                              << ", dirty= " << dirty
                              << ", data= ")
        DEBUG_CACHE(for (int i = 0; i < 4; ++i) std::cout << data[i] << " ")
        DEBUG_CACHE(std::cout << std::endl)

        _tags[block_number] = tag;
        _vbit[block_number] = true;
        _data[block_number] = data;
        _dbit[block_number] = dirty;
    }

    void write_word(int block_number, int word_offset, const std::bitset<22>& tag, word_t data, bool dirty = 0) {
        DEBUG_CACHE(std::cout << "DEBUG_CACHE\t" << "write_word "
                              << "block_number= " << block_number
                              << ", word_offset= " << word_offset
                              << ", tag= " << tag
                              << ", data= " << data
                              << ", dirty= " << dirty << std::endl)
        _tags[block_number] = tag;
        _vbit[block_number] = true;
        _data[block_number][word_offset] = data;
        _dbit[block_number] = dirty;
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

