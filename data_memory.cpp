#include <array>
#include <bitset>

class data_memory
{
    using word_t = std::bitset<32>;
    using block_t = std::array<std::bitset<32>, 4>;

  public:
    data_memory(){
        for (int i = 0; i < 1024; i++){
            _data[i] = 0x0;
        }
    }

    block_t read_block(int word_addr) {

        block_t block;
        int block_start = word_addr - word_addr % 4;

        for(int i = block_start, j = 0; i < block_start + 4; ++i, ++j){
            block[j] = _data[i];
        }

        return block;
    }

    void write_word(int addr, const word_t& write_data) {
        _data[addr] = write_data;
    }

  private:
    std::array<word_t, 1024> _data;
};

