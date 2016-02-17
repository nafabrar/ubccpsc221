//VectorPriorityQueue.hpp
#ifndef _VECTORPRIORITYQUEUE_HPP
#define _VECTORPRIORITYQUEUE_HPP

#include <vector>
#include "BagOfPuzzleStates.hpp"

// A naive implementation of a priority queue, using a vector and
// searching for the max for each remove operation
//
class VectorPriorityQueue : public BagOfPuzzleStates
  {
  public:
    VectorPriorityQueue();
    PuzzleState *remove();
    void add(PuzzleState *element);
    PuzzleState *get_next();
    bool is_empty();
    ~VectorPriorityQueue();

  private:
    vector<PuzzleState *> list;
  };

#endif

