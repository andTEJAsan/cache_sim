#ifndef __CACHE_SIM_HPP__
#define __CACHE_SIM_HPP__
#include<iostream>
#include <iomanip>
#include <ios>
#define llu long long unsigned int
#define endl "\n" 
using namespace std;
#include<climits>
// a single level of a cache will be an array of sets
// each way will be an array of cache_entry, indexed by set number
// each cache_entry will be a struct containing 2 values, the tag and the valid bit
// tag can be larger than 32 bits, so we would use long long llu 
struct Cache_entry
{
    /* cache */
    llu tag ;
    bool valid;
    bool dirty;
};
typedef struct Cache_entry Cache_entry;
struct Set
{
    llu associativity;
    Cache_entry** entries;
    llu* lrutable;
    
    Set() : associativity(1), entries(nullptr), lrutable(nullptr) {}; // default constructor
    Set(llu size){
        associativity=size;
        entries = new Cache_entry*[size]; 
        lrutable = new llu[size];
        for (llu i = 0; i < size; i++)
        {
            /* code */
            entries[i] = new Cache_entry ; 
            entries[i]->valid = false;
            entries[i]->dirty = false;
            lrutable[i] = i; // new - lrutable[i] was less recently used than lrutable[i+1] for all i
           
        }
    };

    llu find_idx(llu* arr, llu value, llu size){
        for (llu i = 0 ; i < size ; i ++){
            if (arr[i] == value) return i ; 
        }
        return -1 ; 
    }
    void update_lru(llu index){ 
        llu found_idx = find_idx((this->lrutable), index, associativity) ; 
        for (llu i = found_idx; i < associativity -1 ; i++)
        {
           (this->lrutable)[i] = (this->lrutable)[i+1] ;  
        } 
        (this->lrutable[associativity-1])=index; 
    };
    llu least_recent(){ // updates lru on being called
        
        llu* temp = (this->lrutable) ; 
        llu return_value = temp[0] ; 
       
        for (llu i = 0 ; i < (this->associativity) -1  ; i ++){
            temp[i] = temp[i+1] ;  
        }
        temp[associativity-1] = return_value  ;
        // cout << "lru - "<< return_value << endl ;  
        return return_value ;  
    };
        
    
};
typedef struct Set Set;



struct Cache_level
{   
        private:
        int get_power(llu value) {
            int i = 0 ; 
            while (value > 0){
                value = value/2 ; 
                i += 1; 
            }
            return i-1 ; 
        }
    
        llu get_value(llu value, int no_of_bits){
            llu a = (value >> no_of_bits) << no_of_bits ; 
            return (value - a) ; 
        }

    
        
    public:
    llu reconstruct(llu tag, llu set_no){
            int block_bit = get_power(blocksize) ; 
           
            int set_index_bit = get_power(no_of_sets) ; 
            
            llu address = (tag << (set_index_bit + block_bit)) + (set_no << (block_bit)) ; 
            return address ; // we dont need the exact address as we are dealing with blocks 
        }

    
    llu reads;
    llu read_misses;
    llu writes;
    llu write_misses;
    llu write_back ; 
    llu blocksize;
    llu cache_size;
    llu associativity;
    // above 3 parameters will be powers of 2
    llu no_of_sets;
    bool is_memory ; 
    Set** cache;

    Cache_level* nextlevel ; Cache_level* prevlevel ; 
    

    Cache_level(){
        reads=0;read_misses=0;writes=0;write_misses=0; write_back = 0 ; 
        blocksize= cache_size = associativity = no_of_sets= 1; 

        nextlevel = prevlevel =  nullptr ;  
        is_memory = false ; cache = nullptr ; 
    }

    Cache_level(llu blsize,llu csize,llu asso, Cache_level* next_level, Cache_level* prev_level, bool ismemory){
        reads=0;
        read_misses=0;
        writes=0;
        write_misses=0; write_back = 0 ; 
        blocksize= blsize ;
        cache_size= csize ;
        associativity = asso ;
        no_of_sets= (csize / blsize) / asso ; 

        nextlevel = next_level ; prevlevel = prev_level ;  
        is_memory = ismemory ; 
        cache = new Set*[no_of_sets];
        for (llu i = 0; i < (no_of_sets) ; i++) { 
            
            cache[i] = new Set(asso);
            }
    }
    void printer(){
        double miss_rate = ((double)(this->write_misses + this->read_misses)  / (double)(this->writes + this->reads)) ; 
        // double read_miss_rate = ((double)(this->read_misses) / (double)(this->reads)) ; 
        cout << "-----------------------------------------------"<< endl;
        cout << "READS : \t\t\t" << (this->reads)<<endl;
        cout << "READ Misses: \t\t\t" << (this->read_misses)<< endl;
        
        
        cout << "WRITES : \t\t\t" << (this->writes) << endl; 
        cout << "WRITE Misses: \t\t\t" << (this->write_misses) << endl;
        cout << "miss rate: \t\t\t" << setprecision(4) << miss_rate << endl ;   
        cout << "WRITE backs: \t\t\t" << (this->write_back) << endl;
        cout << "------------------------------------------------"<<endl;
    }

    

    llu check_hit( llu address){
        // cout << "inside check hit" << endl ; 
        // returns index of hit if there is a hit otherwise returns assoc
        llu tag = (address / blocksize) / no_of_sets;
        llu set_no = (address / blocksize) % no_of_sets;
        Set set = *(cache[set_no]);

        for (llu i = 0; i < associativity; i++) // scanning the set to find the tag
        {
            if((set.entries)[i]->valid && (tag == (set.entries)[i]->tag))  {
                // cout << "tag is " << tag << " set_no is " << set_no <<endl ;
                return i;   
            }
        }
        // cout << "tag is " << tag << " set_no is " << set_no <<endl ;
        return associativity;
    }
    
    void evict(llu address){
        // cout << "evicting address " << address << endl ;
        llu tag = (address / blocksize) / no_of_sets;
        llu set_no = (address / blocksize) % no_of_sets; 
        for (int i = 0 ; i < associativity ; i ++){
            if ( ((*(cache[set_no])).entries)[i]->tag == tag) {
                ( ( (*(cache[set_no])).entries)[i])->valid = false ; 
                // cout << "all ok" << endl ; 
            }
        }
    }

    void read(llu address){
        // cout << "inside read" << endl ;
        reads ++  ; 
        // cout << reads << endl ; 
        

        if(is_memory) return ; 
        llu tag = (address / blocksize) / no_of_sets;
        llu set_no = (address / blocksize) % no_of_sets;
        Set set = *(cache[set_no]);
        llu hit_address = check_hit(address) ;  
        
        // cout <<"checker : "<< hit_address <<endl;
        if (hit_address < associativity){
            // if hit
            // cout << "hit " << "at " << address << endl ; 
            
            // update the lru table        
            llu index = check_hit(address);
            set.update_lru(index);
        }
        else{
            // cout << "miss " << "at " << address << endl ; 
            read_miss(address) ; 
        }
    }

    void read_miss(llu address ){
        read_misses ++ ; 
        llu tag = (address / blocksize) / no_of_sets;
        llu set_no = (address / blocksize) % no_of_sets;
        Set set = *(cache[set_no]);

        llu index_to_replace=set.least_recent(); // updates the lru as well 
        llu evict_address = reconstruct(set.entries[index_to_replace]->tag, set_no) ; 
        Cache_entry* ptr = (set.entries[index_to_replace]) ;
        if (ptr->valid && ptr->dirty){
            (*nextlevel).write(evict_address) ;  // writeback   
            write_back ++ ; 
            
        }
        if (ptr->valid && prevlevel){ // prevlevel null for starting cache
            // cout << "reached here" << endl ;  
            (*prevlevel).evict(evict_address) ; 
            // cout << "all ok ere also" << endl ; 
        } 
        
        (*nextlevel).read(address) ; 
        
        ptr->tag = tag ; ptr->valid = true ; ptr->dirty = false ; 
        
    }

    void write(llu address){
        writes ++ ; 
        if(is_memory) return ; 
        llu tag = (address / blocksize) / no_of_sets;
        llu set_no = (address / blocksize) % no_of_sets;
        // cout << "current set no is " << set_no << endl ; 
        Set set = *(cache[set_no]);
        llu hit_address = check_hit(address) ;
        if (hit_address < associativity) {
            // hit 
            // cout << "hit " <<  " at " << address << endl ; 
            Cache_entry* ptr = (set.entries[hit_address]) ; 
            ptr->dirty = true ; ptr->valid = true ; 
            set.update_lru(hit_address) ; 
        }
        else {
            write_miss(address) ; 
            // cout << "miss " << "at " << address << endl ;  
        }
    } ; 

    void write_miss(llu address){
        write_misses ++ ; 
        llu tag = (address / blocksize) / no_of_sets;
        llu set_no = (address / blocksize) % no_of_sets;
        Set set = *(cache[set_no]);
        llu index_to_replace = set.least_recent() ; // updates the lru as well
        Cache_entry* ptr = set.entries[index_to_replace] ; 
        llu evict_address = reconstruct(ptr->tag, set_no) ; 

        if (ptr->valid && ptr->dirty){
            
            (*nextlevel).write(evict_address)  ; // write_back 
            write_back ++ ; 
            
        }
        if (ptr->valid && prevlevel){ // prevlevel null for starting cache 
                (*prevlevel).evict(evict_address) ; 
        } 
        (*nextlevel).read(address) ; 
        ptr->dirty = true ; ptr->valid = true ; ptr->tag = tag ; 
    }

    



};


#endif