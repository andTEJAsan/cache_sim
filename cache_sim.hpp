#ifndef __CACHE_SIM_HPP__
#define __CACHE_SIM_HPP__

using namespace std;
// a single level of a cache will be an array of ways
// each way will be an array of cache_entry, indexed by set number
// each cache_entry will be a struct containing 2 values, the tag and the valid bit
// tag can be larger than 32 bits, so we would use long long int 
struct cache_entry
{
    /* data */
    long long int tag;
    bool valid;
    bool dirty;
};
typedef struct cache_entry cache_entry;

struct cache_level
{
    int reads;
    int read_misses;
    int writes;
    int write_misses;
    ca
};


#endif