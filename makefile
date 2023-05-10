all : 
	g++ -o cache_simulate cache_sim.cpp cache_sim.hpp ;
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace1.txt >> o.txt 
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace2.txt >> o.txt 
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace3.txt >> o.txt
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace4.txt >> o.txt
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace5.txt >> o.txt
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace6.txt >> o.txt
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace7.txt >> o.txt
	./cache_simulate 64 1024 2 65536 8 ./memory_trace_files/trace8.txt >> o.txt
	
