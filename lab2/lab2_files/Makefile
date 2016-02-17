all : insertion

insertion : insertion.cc insertion.h
	g++ -Wall -g insertion.cc -o insertion

pointers : pointers.cc
	g++ -Wall -g pointers.cc -o pointers

extra : extra.cc
	g++ -Wall -g extra.cc -o extra

clean : 
	rm -f insertion 
	rm -f pointers
	rm -f extra 
	rm -fr insertion.dSYM/* 
	rm -fr pointers.dSYM/* 
	rm -fr extra.dSYM/* 
	rmdir insertion.dSYM
	rmdir pointers.dSYM
	rmdir extra.dSYM
	
