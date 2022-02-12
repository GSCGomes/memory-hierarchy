#include <iostream>
#include <fstream>
#include <string>
#include "data_memory.cpp"
#include "cache.cpp"
#include <bits/stdc++.h>
#include <bitset>
#include <sstream>

#define DEBUG(code) if (1) { code; }

const char* output_file_name = "results.txt";

int main(int argc, char** argv)
{
    int n_hits = 0;
    int n_misses = 0;
    int n_writes = 0;

    data_memory D;
    cache C;

    using word_t = std::bitset<32>;
    using tag_t = std::bitset<22>;
    word_t write_data;
    word_t tag_data;
    std::array<word_t, 4> block_data;

    std::ifstream input_data;

    std::stringstream output;

    if (argc >= 2){
        input_data.open(argv[1]);
    } else {
        std::cerr << "Missing input file.\n";
        return 1;
    }

    std::string row;

    if (input_data.is_open()) {
        while (getline (input_data, row)) {

            DEBUG(std::cout << "\n\n********* row = " << row << std::endl << std::endl)

            std::stringstream ss(row);
            std::string word;

            // gets operation address and calculates block's address and number
            int addr;
            ss >> (addr);

            if (addr >= 4096) {
                std::cerr << "Address " << addr << " is too large. Please use values between 0 and 4095\n";
                return 1;
            }

            if (addr % 4) {
                std::cout << "Address " << addr << " is not divisible by 4. Considering address "
                          << addr - (addr % 4) << ".\n";
            }

            int block_addr, block_number;
            block_addr = trunc(addr/16); //16 bytes per block
            block_number = block_addr % 64; //64 blocks per cache

            // tag is the 22 MSBs from the address
            tag_t tag = (word_t(addr) >> (32-22)).to_ulong();

            int word_addr, word_offset;
            word_addr = (word_t(addr >> 2).to_ulong());  // gets word address
            word_offset = word_addr % 4; // gets word position in a block

            // gets operation, if write evaluates next input term.
            int op;
            ss >> (op);
            if (op != 0 && op != 1) {
                std::cout << "Invalid operation \"" << op << "\". Please enter \"0\" for reading and \"1\" for writing\n";
                return 1;
            }

            auto update_memory = [&](){
                // check if the cache block is dirty and update memory with the pending value
                if(C.is_dirty(block_number)){
                    auto word = C.read_word(block_number, tag, word_offset);
                    if (word.has_value()){
                        D.write_word(word_addr, word.value());// write back
                    }
                }
            };

            if (op == 0){ // reading

                if(C.read_word(block_number, tag, word_offset).has_value()){
                    output << addr << " " << op << " H\n";
                    DEBUG(std::cout << "hit" << std::endl;)
                    ++n_hits;
                }else{
                    output << addr << " " << op << " M\n";
                    DEBUG(std::cout << "miss" << std::endl;)
                    ++n_misses;
                    block_data = D.read_block(word_addr);

                    update_memory();
                    C.write_block(block_number, tag, block_data);
                }
            }else{ // writing
                ++n_writes;
                ss >> (write_data);
                output << addr << " " << op << " " << write_data << " W\n";

                update_memory();

                // we can't write directly to only one word of the cache
                // first we make sure that the rest of the block is filled correctly
                if(!C.read_word(block_number, tag, word_offset).has_value()){
                    block_data = D.read_block(word_addr); // #TODO alocating a word, but should be a block
                    C.write_block(block_number, tag, block_data);
                }

                C.write_word(block_number, word_offset, tag, write_data, true /* dirty */);
            }
        }

    } else {
        std::cerr << "Couldn't open file \"" << argv[1] << "\"\n";
        return 1;
    }

    C.dump();
    D.dump();

    std::ofstream output_file;
    output_file.open(output_file_name);
    if (!output_file.is_open()){
        std::cerr << "Couldn't write output to file \"" << output_file_name << "\"\n";
        return 1;
    }

    int n_reads = n_hits + n_misses;
    output_file << "READS: " << n_reads << "\n";
    output_file << "WRITES: " << n_writes << "\n";
    output_file << "HITS: " << n_hits << "\n";
    output_file << "MISSES: " << n_misses << "\n";
    output_file << "HIT RATE: " << (double) n_hits/ (double) (n_reads) << "\n";
    output_file << "MISS RATE: " << (double) n_misses/ (double) (n_reads) << "\n\n\n";

    output_file << output.str();
    output_file.close();

    return 0;
}

//block address = addr div 16
//block number  = block address mod 64
//word position = byte offset div 4/ Word position = byte offset div 4
