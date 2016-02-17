//HeapPriorityQueue.hpp
#ifndef _HEAPPRIORITYQUEUE_HPP
#define _HEAPPRIORITYQUEUE_HPP

// For this class (but not the your queue and stack classes), you
// are allowed to use the C++ STL vector class.  This saves you from
// having to resize the array when it fills up.
#include <vector>

#include "BagOfPuzzleStates.hpp"

// An implementation of a priority queue, using
// a min-heap as the underlying data structure.
//
// The priority should be based on each PuzzleState's getBadness() value.

class HeapPriorityQueue : public BagOfPuzzleStates
  {
  public:
    HeapPriorityQueue();
    PuzzleState *remove();
    void add(PuzzleState *element);
    PuzzleState *get_next();
    bool is_empty();
    ~HeapPriorityQueue();
    
  private:
    // TODO: add any private member variables you need
    // and any additional helper functions.
    // 
    // We suggest a member variable using the C++ STL
    // type vector<PuzzleState *> to hold the heap.
    // You can look at the VectorPriorityQueue class
    // for an example of how to use the C++ vector<T> class.

    // Heap-related helper functions.

    // The array representation of the heap.
  };

#endif
