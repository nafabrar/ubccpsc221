#ifndef _MINHEAP_H_
#define _MINHEAP_H_

#define DEFAULT_CAPACITY 256 

class Minheap {
public:
    Minheap();
    Minheap(int capacity);
    Minheap(int* elements, int num_elements);
    ~Minheap();

    /* add element to the heap */
    void insert(int element);
    /* remove the top element and return its value */ 
    int delete_min();
    /* remove all elements with value equal to value-to-remove */
    void remove_matching(int value_to_remove);
    
    /* prints the heap array */
    void print_array();
    /* print the heap as a tree */ 
    void print_tree(); 
     
    /* returns the number of elements currently in the heap */
    int get_size(); 
    /* returns the maximum number of elements that can be in the heap */
    int get_capacity();
    /* returns the heap as a new array (of capacity = _capacity) */ 
    int* get_heap(); 

    /* returns true IFF _size == 0 */
    bool isEmpty(); 
    
private:
	int _capacity;
	int _size;
	int* _heap;
	
    void print_tree(int ix, int level);  
    void heapify();
    void swap_down(int ix);	
    void swap_up(int ix);	
    
};

#endif /* _MINHEAP_H_ */
