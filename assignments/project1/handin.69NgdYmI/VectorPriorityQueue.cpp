#ifndef _VECTORPRIORITYQUEUE_CPP
#define _VECTORPRIORITYQUEUE_CPP

//VectorPriorityQueue.cpp
#include "VectorPriorityQueue.hpp"
#include <cassert>
#include <cstdlib>//for NULL
#include <iostream>

// A naive implementation of a priority queue, using
// a vector and searching for the max for each remove operation.
//
// Priority is based on the each PuzzleState's getBadness() method.

VectorPriorityQueue::VectorPriorityQueue() {
  // empty, nothing to do
}

VectorPriorityQueue::~VectorPriorityQueue() {
  // no clean-up to do, since list is not dynamically allocated
}

void VectorPriorityQueue::add(PuzzleState *elem) {
  list.push_back(elem);
}

PuzzleState *VectorPriorityQueue::remove() {
  if (list.size() < 1)
    return (PuzzleState *)NULL;

  int min_index = 0;
  for (int i = 0; i < (int)list.size(); i++) {
    if (list[i]->getBadness() < list[min_index]->getBadness())
      min_index = i;
  }

  PuzzleState *ret = list[min_index];
  list.erase(list.begin() + min_index); // remove list[min_index]
  return ret;
}


PuzzleState *VectorPriorityQueue::get_next() {
  if (list.size() < 1)
    return (PuzzleState *)NULL;

  int min_index = 0;
  for (int i = 0; i < (int)list.size(); i++) {
    if (list[i]->getBadness() < list[min_index]->getBadness())
      min_index = i;
  }

  return list[min_index];
}

bool VectorPriorityQueue::is_empty() {
  return (list.size() == 0);
}

#endif 
