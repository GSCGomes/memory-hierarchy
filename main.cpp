#include <iostream>
#include <fstream>
#include <string>
#include "data_memory.cpp"
#include "cache.cpp"
#include <bits/stdc++.h>
#include <bitset> 
#include <sstream>

#define DEBUG(code) if (1) { code; }

int main(int argc, char** argv)
{

    int addr, op, block_addr, block_number, word_addr, word_offset;
    int hit_rate = 0;
    int miss_rate = 0;
    
    data_memory D;
    cache C;

    C.init();// init memories
    D.init();
    
    using word_t = std::bitset<32>;
    word_t write_data;
    word_t tag_data;
    std::array<word_t, 4> block_data;

    std::ifstream input_data; 

    if (argc >= 2){
        input_data.open(argv[1]);
    } else {
        std::cout << "Missing input file.\n";
        return 1;
    }

    std::string row;

    if (input_data.is_open()) {
        while (getline (input_data, row)) {

            DEBUG(std::cout << "\n\n********* row = " << row << std::endl << std::endl)

            std::stringstream ss(row);
            std::string word;
            
            // Gets operation address and calculates block's address and number
            ss >> (addr);
            block_addr = trunc(addr/16); //16 bytes per block
            block_number = block_addr % 64; //64 blocks per cache
           

            std::bitset<32> baddr(addr);
            std::bitset<22> tag;
            
            for (int i = 21; i >= 0; i--){ // change this for suggested hack by fish
                tag[i] = baddr[i+10];
            }   

            
            word_addr = (std::bitset<32>(addr >> 2).to_ulong());  // gets word addres
            word_offset = word_addr % 4; // gets wrod postion in a block

            // Gets operation, if write evaluates next input term.
            ss >> (op);

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
                    DEBUG(std::cout << "hit" << std::endl;)
                    hit_rate++;
                }else{
                    DEBUG(std::cout << "miss" << std::endl;)
                    miss_rate++;
                    block_data = D.read_block(word_addr);

                    update_memory();
                    C.write_block(block_number, tag, block_data);
                }          
            }else{ // writing
                ss >> (write_data);

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
        std::cout << "Couldn't open file \"" << argv[1] << "\"\n";  
        return 1;
    } 

    C.dump();
    D.dump();

    std::cout << "Miss Rate: " << miss_rate << std::endl;
    std::cout << "Hit Rate:  " << hit_rate << std::endl;
    return 0;
}

//block address = addr div 16
//blcok number  = block addres mod 64
// Word position = byte offset div 4/ Word position = byte offset div 4
