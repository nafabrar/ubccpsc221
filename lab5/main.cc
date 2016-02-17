#include <iostream>
#include "Minheap.h"
#include "Unit.h"

void run_me_first(void){ // any test code of your own, runs before unit tests 
	// whatever you like 
}
/* ******** DO NOT CHANGE BEYOND HERE ******** */ 

int input1[] = { 8, 3, 5, 6, 2, 9, 1, 7, 4, 0 };
int input2[] = { 4, 6, 1, 8, 2, 3 };

void print_input(int* src, int size){
	for (int ii = 0; ii < size; ++ii) {
		std::cout << src[ii] << " ";
	}
	std::cout << std::endl; 
}
int check_heap_property( Minheap* minheap ){ 
	int* copy1 = minheap->get_heap();
	int size1 = minheap->get_size(); 
	int result = 0;
	for (int ii=1; ii < size1; ++ii) {
		int parent = (ii - 1) /2;
		if (copy1[parent] > copy1[ii]) result += 1; 
	}
	delete[] copy1; 
	return result; 
}
int main() {
	run_me_first(); 
	
	Unit unit; 
	const int size1 = sizeof(input1) / sizeof(int);
	std::cout << "input1: "; 
	print_input( input1, size1 ); 
	Minheap* heap1 = new Minheap(input1, size1);
	std::cout << "heap1: " << std::endl;
	heap1->print_tree();
	heap1->print_array();
	int heap1_size = heap1->get_size(); 
	unit.assertEquals("Heap size", size1, heap1_size );
	unit.assertEquals("Heap capacity", size1, heap1->get_capacity() );
	int num_Parent_GT_child = check_heap_property( heap1 ); 
	unit.assertEquals("heap1: num parents GT child", 0, num_Parent_GT_child);
	
	// delete everything
	int prev = heap1->delete_min();
	int min_value = prev; 
	heap1_size --; 
	for (int ii=1; ii <size1; ++ii) { 
		unit.assertEquals("size after delete_min", heap1_size, heap1->get_size() );
		num_Parent_GT_child = check_heap_property( heap1 ); 
		unit.assertEquals("invalid heap after delete_min", 0, num_Parent_GT_child);
		int curr = heap1->delete_min();
		unit.assert_LE("Sequence of deleted values", prev, curr );
		prev = curr; 
		heap1_size --; 
	}
	unit.assertEquals("Heap size", 0, heap1_size );
	int max_value = prev; 
	
	Minheap* heap2 = new Minheap(64); 
	unit.assertEquals("Heap size", 0, heap2->get_size() );
	unit.assertEquals("Heap capacity", 64, heap2->get_capacity() );
	// insert everything back on 
	for (int ii=0; ii < size1; ++ii) {
		heap2->insert( input1[ii] ); 
		heap1_size ++; 
		unit.assertEquals("size after insert", heap1_size, heap2->get_size() );
		num_Parent_GT_child = check_heap_property( heap2 ); 
		unit.assertEquals("heap2: invalid heap after insert", 0, num_Parent_GT_child);
	}
	// insert several copies of largest value 
	heap2->insert( max_value ); 
	heap2->insert( max_value ); 
	heap2->insert( max_value ); 
	heap2->insert( max_value ); 
	unit.assertEquals("size after duplicates inserted", heap1_size + 4, heap2->get_size() );
	num_Parent_GT_child = check_heap_property( heap2 ); 
	unit.assertEquals("heap2: invalid heap after duplicates", 0, num_Parent_GT_child);
	heap2->remove_matching( max_value ); 
	heap1_size --; 
	unit.assertEquals("size after remove_matching (5) ", heap1_size, heap2->get_size() );
	heap1->remove_matching( max_value ); 
	unit.assertEquals("size after remove_matching (none) ", heap1_size, heap2->get_size() );
	num_Parent_GT_child = check_heap_property( heap2 ); 
	unit.assertEquals("heap2: invalid heap after remove_matching", 0, num_Parent_GT_child);
		
	// insert several copies of smallest value 
	heap2->insert( min_value ); 
	heap2->insert( min_value ); 
	unit.assertEquals("size after duplicates inserted", heap1_size + 2, heap2->get_size() );
	heap2->remove_matching( min_value ); 
	heap1_size --; 
	unit.assertEquals("size after remove_matching (3) ", heap1_size, heap2->get_size() );
	heap1->remove_matching( min_value ); 
	unit.assertEquals("size after remove_matching (0) ", heap1_size, heap2->get_size() );
	num_Parent_GT_child = check_heap_property( heap2 ); 
	unit.assertEquals("heap2: invalid heap 2nd remove_matching", 0, num_Parent_GT_child);

	std::cout << std::endl;
	unit.printResults(); 
	
	delete heap1;
	delete heap2;

	return 0;
}
