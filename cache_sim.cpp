#include<iostream>
#include "cache_sim.hpp"
using namespace std;

int main(){
    Cache_level L1(4,8*8,2,8);
    L1.printer();
    L1.read_request(static_cast<long long int>(0b00000),nullptr);
    //2 bits are offset, 3 are index
    L1.read_request(static_cast<long long int>(0b00001),nullptr);
    L1.read_request(static_cast<long long int>(0b00001),nullptr);
    // should be hits
    L1.printer();
    // should have 3 reads, 1 read miss
    L1.read_request(static_cast<long long int>(0b00100),nullptr);
    L1.printer();
    L1.read_request(static_cast<long long int>(0b00110),nullptr);
    L1.printer();
    return 0;

}