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

    int addr, op, block_addr, block_number, word_offset;
    
    data_memory D;
    cache C;
       
    std::bitset<32> write_data;
    std::bitset<4> byte_offset;
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

            word_offset = (std::bitset<4>(addr).to_ulong())/4;  // gets four last bits from addres and gets word possition in a block, four words per block    
        
            // Gets operation, if write evaluates next input term.
            ss >> (op);
            if (op == 0){
                std::cout << "Read Op" << "\n";
                //C.read(block_number, word_offset);            
            }else{
                ss >> (write_data);            
                std::cout << write_data << "\n";
                //C.read(block_number, word_offset);
                std::cout << "Write Op" << "\n";
            }
        }

    }else {
        std::cout << "Couldn't open file\n";  
    } 

    std::cout << "size of data memory: " << sizeof(D) << " bytes"
              << "\nsize of cache: " << sizeof(C) << " bytess"
              << std::endl;
    return 0;
}


//block address = addr div 16
//blcok number  = block addres mod 64
// Word position = byte offset div 4