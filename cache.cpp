#include <array>
#include <bitset>
#include <optional>

class cache
{
    using word_t = std::bitset<32>;
    using tag_t = std::bitset<22>;
    using block_t = std::array<std::bitset<32>, 4>;

  public:
    cache(){
        for (int i = 0; i < 64; i++){
            for (int j = 0; j < 4; j++){
                _data[i][j] = 0x0;
            }
            _tags[i] = 0x0;
            _vbit[i] = 0x0;
            _dbit[i] = 0x0;
        }
    }

    std::optional<word_t> read_word(int block_number, const tag_t& tag, int word_offset) {

        if(_vbit[block_number] && (_tags[block_number] == tag)){
            return _data[block_number][word_offset];
        }
        return {};
    }

    bool is_dirty(int block_number) {  // Method/function defined inside the class
        return _dbit[block_number];
    }

    void write_block(int block_number, const tag_t& tag, block_t data, bool dirty = 0) {
        _tags[block_number] = tag;
        _vbit[block_number] = true;
        _data[block_number] = data;
        _dbit[block_number] = dirty;
    }

    void write_word(int block_number, int word_offset, const tag_t& tag, word_t data, bool dirty = 0) {
        _tags[block_number] = tag;
        _vbit[block_number] = true;
        _data[block_number][word_offset] = data;
        _dbit[block_number] = dirty;
    }

  private:
    std::array<block_t, 64> _data;

    //each tag or validity bit represents a block
    std::array<tag_t, 64> _tags;
    std::array<bool, 64> _vbit;
    std::array<bool, 64> _dbit;

};

