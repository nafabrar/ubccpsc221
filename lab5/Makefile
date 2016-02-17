all: main.cc Minheap.cc Minheap.h Unit.cc Unit.h  
	g++ -Wall -c Unit.cc -o Unit.o 
	g++ -Wall -c Minheap.cc -o Minheap.o 
	g++ -Wall -g main.cc Minheap.o Unit.o -o minheap

\clean:
	rm -f Minheap.o 
	rm -f Unit.o 
	rm -f minheap 
	rm -f example 
	rm -fr minheap.dSYM/* 
	rmdir minheap.dSYM 
