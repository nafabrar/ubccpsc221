#ifndef _AVLDICT_CPP
#define _AVLDICT_CPP

//AVLDict.cpp
#include "AVLDict.hpp"
#include <cassert>
#include <cstdlib>//for NULL
#include <iostream>

// An implementation of a dictionary ADT as an unsorted linked list.
//
AVLDict::AVLDict() {
  root = NULL;

  // Initialize array of counters for depth statistics
  depth_stats = new int[MAX_STATS]();
}

void AVLDict::destructor_helper(node * x) {
  if (x == NULL) return; // Nothing to destroy.
  destructor_helper(x->left);
  destructor_helper(x->right);
  delete x->key;
  delete x;
}

AVLDict::~AVLDict() {
  // Clean up the tree.
  // This is most easily done recursively.
  destructor_helper(root);

  // It's not good style to put this into a destructor,
  // but it's convenient for this assignment...
  cout << "Depth Statistics for find():\n";
  for (int i=0; i<MAX_STATS; i++)
    cout << i << ": " << depth_stats[i] << endl;
  delete [] depth_stats;
}

bool AVLDict::find_helper(node *r, string keyID, PuzzleState *&pred, int depth) {
  if (r==NULL) {
    if (depth<MAX_STATS) depth_stats[depth]++;
    return false; // Didn't find it.
  }
  if (keyID == r->keyID) {
    if (depth<MAX_STATS) depth_stats[depth]++;
    pred = r->data; // Got it!  Get the result.
    return true;
  }
  if (keyID < r->keyID) return find_helper(r->left, keyID, pred, depth+1);
  else return find_helper(r->right, keyID, pred, depth+1);
}

bool AVLDict::find(PuzzleState *key, PuzzleState *&pred) {
  return find_helper(root, key->getUniqId(), pred, 0);
}

bool AVLDict::update_height( node * x ) {
  //
  // Recalculates the height of x from the height of its children.
  // Returns true iff the height of x changes.
  //

  // :TODO: Write this function!
  return false; // Remove this line when you've implemented this function.
}

void AVLDict::rotate_left( node *& a ) {
  // "rotates" the subtree rooted at a to the left (counter-clockwise)

// 221 Students:  DO NOT CHANGE OR DELETE THE NEXT FEW LINES!!!
// We will use this code when marking to be able to watch what
// your program is doing, so if you change things, we'll mark it wrong.
#ifdef MARKING_TRACE
std::cout << "Rotate Left: " << a->keyID << std::endl;
#endif
// End of "DO NOT CHANGE" Block

  // :TODO: Write this function!
}

void AVLDict::rotate_right( node *& b ) {
  // "rotates" the subtree rooted at b to the right (clockwise)

// 221 Students:  DO NOT CHANGE OR DELETE THE NEXT FEW LINES!!!
// We will use this code when marking to be able to watch what
// your program is doing, so if you change things, we'll mark it wrong.
#ifdef MARKING_TRACE
cout << "Rotate Right: " << b->keyID << endl;
#endif
// End of "DO NOT CHANGE" Block

  // :TODO: Write this function!
}

// You may assume that no duplicate PuzzleState is ever added.
void AVLDict::add(PuzzleState *key, PuzzleState *pred) {

  // :TODO: Write this function!
}

#endif 
