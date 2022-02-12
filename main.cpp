#include <iostream>
#include <fstream>
#include <string>
#include "data_memory.cpp"
#include "cache.cpp"
#include <bits/stdc++.h>
#include <bitset> 
#include <sstream>

int main()
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
    std::array<word_t, 4> _block_data;

    std::ifstream input_data; 

    input_data.open("input.txt");
    std::string row;

    if (input_data.is_open()) {
        while (getline (input_data, row)) {

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
                                  
            if (op == 0){
                if(C.read(block_number, tag)){
                    std::cout << "hit" << std::endl;
                    hit_rate++;
                }else{
                    std::cout << "miss" << std::endl;
                    miss_rate++;
                    _block_data = D.read(word_addr); // #TODO alocating a word, but should be a block
                    C.write_from_memory(block_number, tag, _block_data);
                }          
            }else{
                ss >> (write_data);
                if(!C.get_dbit(block_number)){ //verifies dirty bits
                    C.write(block_number, word_offset, tag, write_data);
                }else{
                    D.write(word_addr, C.get_data(block_number, word_offset));// write back
                    C.write(block_number, word_offset, tag, write_data);
                }
            }
        }

    }else {
        std::cout << "Couldn't open file\n";  
    } 

    C.dump();
    D.dump();

    std::cout << "size of data memory: " << sizeof(D) << " bytes"
              << "\nsize of cache: " << sizeof(C) << " bytess"
              << std::endl;
    
    std::cout << "Miss Rate: " << miss_rate << std::endl;
    std::cout << "Hit Rate:  " << hit_rate << std::endl;
    return 0;
}


//block address = addr div 16
//blcok number  = block addres mod 64
// Word position = byte offset div 4/ Word position = byte offset div 4
