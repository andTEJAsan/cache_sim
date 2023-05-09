#ifndef __CACHE_SIM_HPP__
#define __CACHE_SIM_HPP__

using namespace std;
// a single level of a cache will be an array of sets
// each way will be an array of cache_entry, indexed by set number
// each cache_entry will be a struct containing 2 values, the tag and the valid bit
// tag can be larger than 32 bits, so we would use long long int 
struct Cache_entry
{
    /* data */
    long long int tag;
    bool valid;
    bool dirty;
};
typedef struct Cache_entry Cache_entry;
struct Set
{
    int associativity;
    Cache_entry* entries;
    int* lrutable;
    
    Set() : associativity(0), entries(nullptr), lrutable(nullptr) {} // default constructor
    Set(int size){
        associativity=size;
        entries = new Cache_entry[size];
        lrutable = new int[size];
        for (int i = 0; i < size; i++)
        {
            /* code */
            entries[i].valid = false;
            entries[i].dirty = false;
            lrutable[i] = size-1; // lrutable is initialized with the maximum value because we would always increment the counter, as we haven't used any set it should be infinity, but since the time cant be more than the size-1, we set it to size-1

        }
        
    }
};
typedef struct Set Set;



struct Cache_level
{
    int reads;
    int read_misses;
    int writes;
    int write_misses;
    
    int blocksize;
    int cache_size;
    int assoc;
    // above 3 parameters will be powers of 2
    int no_of_sets;
    Set* data;
    //
    Cache_level(int blsize,int csize,int asso,int nosets){
        blocksize=blsize;
        cache_size=csize;
        assoc=asso;
        no_of_sets=nosets;
        data = new Set[no_of_sets];
        for (int i = 0; i < assoc; i++)
        {
            data[i] = Set(assoc);
        }
        
    }
    int check_hit(long long int adress){
        // returns index of hit if there is a hit otherwise returns assoc
        long long int tag = (adress / blocksize) / no_of_sets;
        int set_no = (adress / blocksize) % no_of_sets;
        Set set = data[set_no];

        for (int i = 0; i < assoc; i++) // scanning the set to find the tag
        {
            if(set.entries[i].valid && (tag == set.entries[i].tag)) return i;   
        }
        return assoc;
    }
    void read_request(long long int adress, Cache_level* next_level){
        // I have made a second parameter to issue request to a higher level cache, if no higher level
        // then pass NULL
        
        if (check_hit(adress)){
            reads++;
            // update the lru table
        long long int tag = (adress / blocksize) / no_of_sets;
        int set_no = (adress / blocksize) % no_of_sets;
        Set set = data[set_no];
        int index = check_hit(adress);
        for (int i = 0; i < assoc; i++)
        {
            set.lrutable[i]++;
            if (set.lrutable[i] >= (assoc -1)) set.lrutable[i] = assoc-1;
            if(i==index) set.lrutable[i]=0;
        }
        
        
        }
        else{
            if(next_level == nullptr){
                read_misses++;
                reads++;
            }
        }
    }


};


#endif