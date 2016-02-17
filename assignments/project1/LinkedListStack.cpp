//LinkedListStack.cpp
#ifndef _LINKEDLISTSTACK_CPP
#define _LINKEDLISTSTACK_CPP

// We suggest at least these includes:
#include "LinkedListStack.hpp"
#include <cstdlib> 
#include <cassert>
#include <iostream>


LinkedListStack::LinkedListStack()
{
  top = NULL;
}

void LinkedListStack::add(PuzzleState *elem)
{
  node * temp = new node;
  temp->data = elem;
  temp->next = top;
  top = temp;
}

PuzzleState *LinkedListStack::remove()
{
  assert(top != NULL);
  PuzzleState *ret = top->data;
  node* temp = top->next;
  delete top;
  top = temp;
  return ret;
}

PuzzleState *LinkedListStack::get_next()
{
  assert(top != NULL);
  return top->data;
}

bool LinkedListStack::is_empty()
{
  return top==NULL;
}

LinkedListStack::~LinkedListStack()
{
  while(top != NULL)
    remove();
}

#endif
