#include<iostream>
#include "cache_sim.hpp"
using namespace std;

int main(){
    // Cache_level* l1 = new Cache_level() ; Cache_level* l2 ; ; Cache_level* mem = new Cache_level() ; 
    Cache_level* L1 = new Cache_level(4,2*4*8,2, nullptr, nullptr, false) ; 
    Cache_level* L2 = new Cache_level(4, 4*16, 1, nullptr, nullptr, false) ; 
    Cache_level* MEM = new Cache_level(1,1,1,nullptr, nullptr, true) ; 

    L1->nextlevel = L2 ; L2->nextlevel = MEM ; L2->prevlevel = L1 ; MEM->prevlevel = L2  ;  
    
    
    // cout << ( *(L1.nextlevel) == L2) << "\n" ; 
    L1->read(0); 
    L1->printer() ;
    L1->read(0) ;  
    L1->printer() ; 
    // 2 bits are offset, 3 are index
    L1->read(static_cast<long long int>(0b00001));
    L1->read(static_cast<long long int>(0b00011));
    // should be hit1
    L1->printer();
    // should have 3 reads, 1 read miss
    L1->read(static_cast<long long int>(0b00100));
    L1->printer();
    L1->read(static_cast<long long int>(0b00110));
    L1->printer();
    L1->read(static_cast<long long int>(0b0100000)); // should go to 2nd way of set0 // should be a compulsory miss
    L1->printer();
    L1->read(static_cast<long long int>(0b100001)); // should be a hit
    L1->printer();
    L1->read(static_cast<long long int> (0b1000000)); // way with tag 00 at set index 0 should get evicted if LRU is working properly
    L1->printer();
    L1->read(static_cast<long long int>(0b0)); // should be a miss
    L1->printer();
    L1->read(static_cast<long long int>(0b01000000)); // should be a hit
    L1->printer();

    L1->write(static_cast<long long int>(0b01001100)) ; // first miss in L1, L2
    L1->printer();
    L1->write(static_cast<long long int>(0b00001100)) ; // first miss in L1,L2. Invalidates above address 
    L1->printer();
    L1->write(static_cast<long long int>(0b01001100)) ;  //  should be miss 
    L1->printer();


    // some problems in the LRU thing are there
    // debug by printing the tag sets



    return 0;

}