#include<iostream>
#include "cache_sim.hpp"
using namespace std;

int main(){
    Cache_level L1(4,2*4*8,2,8);
    L1.printer();
    L1.read_request(static_cast<long long int>(0b00000),nullptr);
    //2 bits are offset, 3 are index
    L1.read_request(static_cast<long long int>(0b00001),nullptr);
    L1.read_request(static_cast<long long int>(0b00011),nullptr);
    // should be hit1
    L1.printer();
    // should have 3 reads, 1 read miss
    L1.read_request(static_cast<long long int>(0b00100),nullptr);
    L1.printer();
    L1.read_request(static_cast<long long int>(0b00110),nullptr);
    L1.printer();
    L1.read_request(static_cast<long long int>(0b0100000),nullptr); // should go to 2nd way of set0 // should be a compulsory miss
    L1.printer();
    L1.read_request(static_cast<long long int>(0b100001),nullptr); // should be a hit
    L1.printer();
    L1.read_request(static_cast<long long int> (0b1000000),nullptr); // way with tag 00 at set index 0 should get evicted if LRU is working properly
    L1.printer();
    L1.read_request(static_cast<long long int>(0b0),nullptr); // should be a miss
    L1.printer();
    L1.read_request(static_cast<long long int>(0b0100000),nullptr); // should be a hit
    L1.printer();
    // some problems in the LRU thing are there
    // debug by printing the tag sets



    return 0;

}