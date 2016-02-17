#ifndef _BAGOFPUZZLESTATES_HPP
#define _BAGOFPUZZLESTATES_HPP

#include "PuzzleState.hpp"
// generally not good to use a specific namespace in headers,
// for re-usability reasons, but we'll make an exception for
// this project
//using namespace std;

// BagOfPuzzleStates is a base interface representing a bunch of PuzzleStates
// that does not allow random-access semantics, but rather
// only addition and removal of elements, whose ordering
// in a concrete implementation will determine the ADT
// that is implemented (e.g. FIFO order -> queue)
class BagOfPuzzleStates {
 public:
  // a destructor declared as virtual in the
  // class declaration is given an implementation
  // so that the C++ "delete" operator on a subtype
  // will propagate up the type hierarchy into the
  // parent classes
  inline virtual ~BagOfPuzzleStates() { }

  // return the next element, removing it from the list
  virtual PuzzleState *remove() = 0;

  // add an element to the todo list
  virtual void add(PuzzleState *element) = 0;

  // returns whether or not there is at least 1 item in the list
  virtual bool is_empty() = 0;
};

#endif
