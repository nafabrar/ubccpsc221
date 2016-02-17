//ArrayQueue.hpp
#ifndef _ARRAYQUEUE_HPP
#define _ARRAYQUEUE_HPP

#include "BagOfPuzzleStates.hpp"

// This skeleton file (along with the .cpp) is
// here to help get you started, but you are
// free to change function signatures, etc.
// as long as ArrayQueue is some subtype
// of BagOfPuzzleStates
class ArrayQueue: public BagOfPuzzleStates
{
 public:
  ArrayQueue(); // constructor
  
  // TodoList interface
  void add(PuzzleState *elem);
  PuzzleState *remove();
  bool is_empty();

  ~ArrayQueue(); // destructor

 private:
  // A helper function that consumes a number and ensures the queue
  // has enough space for that many elements.
  void ensure_capacity(int n);

  // a pointer to the underlying array with elements of type (PuzzleState *)
  // use an explicit pointer, NOT PuzzleState *array[..] so you can resize.
  PuzzleState **array;

  // TODO: list additional private member variables here,
};

#endif
