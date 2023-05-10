#include<iostream>
#include<fstream> 
#include <bits/stdc++.h>
#include <iomanip>
#include <ios>
#include "cache_sim.hpp"

using namespace std;

vector<string> tokenize(string str, char delim) {
    vector<string> vi ; llu length = 0 ; 
    for (int i = 0 ; i < str.size() ; i ++){
        if(str[i] == delim){
            vi.push_back( str.substr(i-length, length)) ; 
            length  =0 ; 
        }
        else{length ++ ; }
    }  
    vi.push_back(str.substr(str.size()  - length , length) ) ; 
    return vi ; 
}

void  print_vector(vector<string> vi){
    for (auto i = vi.begin() ; i != vi.end() ; i ++){
        cout << (*i) << " "  ; 
    }
    cout << endl ; 
}

int main(int argc, char** argv){

    llu l1_access_time = 1 ; llu l2_access_time = 20 ; llu dram_access_time = 200 ; 

    llu blksize = stoul(argv[1],nullptr,10) ; 
    llu l1_size = stoul(argv[2], nullptr, 10) ; 
    llu l1_assoc = stoul(argv[3], nullptr, 10) ; 

    llu l2_size = stoul(argv[4], nullptr, 10) ; 
    llu l2_assoc = stoul(argv[5], nullptr, 10) ;
    string filename = argv[6] ; 


    Cache_level* L1 = new Cache_level(blksize,l1_size,l1_assoc, nullptr, nullptr, false) ; 
    Cache_level* L2 = new Cache_level(blksize, l2_size, l2_assoc, nullptr, nullptr, false) ; 
    Cache_level* MEM = new Cache_level(1,1,1,nullptr, nullptr, true) ; 

    L1->nextlevel = L2 ; L2->nextlevel = MEM ; L2->prevlevel = L1 ; MEM->prevlevel = L2  ; 


    string line ; llu address ; 
    ifstream myfile (filename); int is_command = 0 ; vector<string> vi ; 
    
        if (myfile.is_open())
        {
            while ( getline (myfile,line) ) {
                vi = tokenize(line, '\t') ; 

                address = stoul(vi[1], nullptr, 16) ; 
                if (vi[0] == "r") {
                    // cout << "read" << endl ; 
                    L1->read(address) ;  
                } 
                else if (vi[0] == "w"){
                    // cout << "write" << endl ; 
                    L1->write(address) ;  
                }
            } ; 
            // L1->printer() ; 
            // L2->printer() ; 
            llu time = 0 ; 
            time += (L1->reads + L1->writes)* l1_access_time; 
            time +=  (L2->reads + L2->writes)* l2_access_time;
            time += (MEM->reads + MEM->writes)* dram_access_time;
            double missrate1 = ((double)(L1->write_misses + L1->read_misses)  / (double)(L1->writes + L1->reads)) ; 
            double missrate2 = ((double)(L2->write_misses + L2->read_misses)  / (double)(L2->writes + L2->reads)) ; 

            cout << time << " "  ; 
            cout << L1->reads << " "; 
            cout << L1->read_misses << " "; 
            cout << L1->writes << " "; 
            cout << L1->write_misses << " "; 
            cout <<  setprecision(4) << missrate1 << " "; 
            cout << L1->write_back << " "; 
            cout << L2->reads << " "; 
            cout << L2->read_misses << " "; 
            cout << L2->writes << " "; 
            cout << L2->write_misses << " "; 
            cout <<   setprecision(4) << missrate2 << " "; 
            cout << L2->write_back << " "; 
            cout << "\n" ; 

            myfile.close();
        }


     
    

    return 0;

}