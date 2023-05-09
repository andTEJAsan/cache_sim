#ifndef __CACHE_SIM_HPP__
#define __CACHE_SIM_HPP__

using namespace std;
#include<climits>
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
    
    Set() : associativity(0), entries(nullptr), lrutable(nullptr) {}; // default constructor
    Set(int size){
        associativity=size;
        entries = new Cache_entry[size];
        lrutable = new int[size];
        for (int i = 0; i < size; i++)
        {
            /* code */
            entries[i].valid = false;
            entries[i].dirty = false;
            lrutable[i] = INT_MAX; // lrutable is initialized with the maximum value because we would always increment the counter, as we haven't used any set it should be infinity, but since the time cant be more than the size-1, we set it to size-1

        }
    };
    void update_lru(int index){
        for (int i = 0; i < associativity; i++)
        {
            if(this->lrutable[i]<INT_MAX){ 
            (this->lrutable[i])++;
            }
        } 
        (this->lrutable[index])=0;
    };
    int least_recent(){
        int max_ind=0;
        // entries will be unique (unless they have hit the int_max)
        for (int i = 0; i < (this->associativity); i++)
        {
            if(((this->lrutable)[i]) > (((this->lrutable)[max_ind]))) max_ind=i;   
        }
        return max_ind;
        // function will be called like A.least_recent() where A is of type set and will give index of the least recent way in the cache
    };
        
    
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
        reads=0;
        read_misses=0;
        writes=0;
        write_misses=0;
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
    void printer(){
        cout << "-----------------------------------------------"<<endl;
        cout << "READS : \t\t\t" << (this->reads)<<endl;
        cout << "READ Misses: \t\t\t" << (this->read_misses)<< endl;
        cout << "WRITES : \t\t\t" << (this->writes) << endl;
        cout << "WRITE Misses: \t\t\t" << (this->write_misses) << endl;
        cout << "------------------------------------------------"<<endl;
    }
    int check_hit(long long int adress){
        // returns index of hit if there is a hit otherwise returns assoc
        long long int tag = (adress / blocksize) / no_of_sets;
        int set_no = (adress / blocksize) % no_of_sets;
        Set set = data[set_no];

        for (int i = 0; i < assoc; i++) // scanning the set to find the tag
        {
            if((set.entries)[i].valid && (tag == (set.entries)[i].tag))  return i;   
        }
        return assoc;
    }
    
    void read_request(long long int adress, Cache_level* next_level){

        // I have made a second parameter to issue request to a higher level cache, if no higher level
        // then pass NULL
        long long int tag = (adress / blocksize) / no_of_sets;
        int set_no = (adress / blocksize) % no_of_sets;
        Set set = data[set_no];
        cout <<"checker : "<< check_hit(adress)<<endl;
        if (check_hit(adress) < assoc){
            // if hit
            cout << "hit\n";
        reads++;
            // update the lru table        
        int index = check_hit(adress);
        set.update_lru(index);
        }
        else{
            cout << "miss\n";
            if(next_level == nullptr){
                read_misses++;
                reads++;
                int index_to_replace;
                index_to_replace=set.least_recent();
                ((set.entries)[index_to_replace]).tag=tag;
                ((set.entries)[index_to_replace]).valid=true;
                set.update_lru(index_to_replace);
            }
            else{
                read_misses++;
                reads++;
                (* next_level).read_request(adress,nullptr);

                int index_to_replace;
                index_to_replace=set.least_recent();
                ((set.entries)[index_to_replace]).tag=tag;
                ((set.entries)[index_to_replace]).valid=true;
                set.update_lru(index_to_replace);
            }
        }
    }


};


#endif