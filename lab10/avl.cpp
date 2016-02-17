//! @file avl.cpp
//! Parallel Lab version of avl.cc 
 
#include <iostream>
#include <fstream>
#include <iomanip>	    // provides setw()
#include <cstdlib>    	// provides atoi()
#include <cassert>      // provides assert
#include <functional>   // provides std::function

#include "Timer.hpp"

#define REPETITIONS 1       //!< REPETITIONS = the number of times to repeat each "scheme". 
                             //! We can "smooth" the data a little by running several trials back to back, 
                             //! and looking at the total time instead. (Okay as lomg as there's nothing 
                             //! significant between each individual trial.)
                             
int SEQUENTIAL_CUTOFF = 9;   //!< A full binary tree has (2^(n+1) - 1) nodes, so with n = 9, at most (2^10 - 1) = 1023 nodes 
                             //! BUT an AVL tree is rarely full, so the end of this lab has you trying to find a more reasonable
                             //! number for this. 

typedef std::string KType;   //!< @typedef std::string KType. The datatype for the key of the AVL tree Node
typedef int VType;           //!< @typedef int VType. The datatype for the value of the AVL tree Node
             
struct Node {        //!< A Node in an AVL tree. We're using it to tally the number of times each word is used in the input. 
  KType key;         //!< the key of the node (i.e. a word) 
  VType value;       //!< the value of the node (i.e. the number of times the word occurs in the input)
  int height;        //!< the height of the node 
  Node * left;       //!< pointer to the left child of the node (or NULL) 
  Node * right;      //!< pointer to the right child of the node (or NULL) 
};

/* function prototypes */
Node*& findNode(KType key, Node*& root); 
void insert( KType key, VType value, Node *& root );
Node* createNode( KType key, VType value, Node* left = NULL, Node* right = NULL );

void balance( Node *& root );
void doubleRotateLeft( Node *& root );
void rotateLeft( Node *& root );
void doubleRotateRight( Node *& root );
void rotateRight( Node *& root );

int calcHeight( Node * node );
int getHeight( Node * node );
void setHeight( Node * node, int height );

int getValue( Node * node );
void setValue( Node * node, VType value );

// NOTE HOW IT PASSES A FUNCTION AS A PARAMETER 
void findMostCommonAndReport(std::function<void(Node*, KType&, VType&)> f,
    Node * tree, KType & k, VType & v, const char * type); 

void findMax(Node * root, KType & word, VType & count);
void findMaxHelper(Node * root, KType & word, VType & count);
void findMaxSequential(Node * root, KType & word, VType & count);

void deleteTree( Node*& root );

void usage(const char* rununit); 

// checking your code doesn't violate the properties of the data structures 
// you're using is a good way to catch bugs
bool isValidAVLTree( Node* root ); 

// the rest of these are not used, just here for completeness and comparison 
void printTreeHelper( Node * node, int depth );
void printTree( Node * node );

bool hasKey( KType key, Node * node );
VType& lookup( KType key, Node * root ); 


/**
 * Creates a new Node with the given key and value. 
 * 
 * If left and right are supplied, then they become the left and right children 
 * of the new Node, otherwise they default to NULL and the new Node has no children.  
 *
 * @param key = the key of the new Node
 * @param value = the value for the new Node
 * @param left = the address of the Node that will be the left child of the new Node, 
 *        defaults to NULL
 * @param right = the address of the Node that will be the right child of the new Node,
 *        defaults to NULL
 * 
 * @pre  key is a valid KType 
 * @pre  value is a valid VType 
 * @pre  (left == NULL) or *left is a valid Node 
 * @pre  (right == NULL) or *right is a valid Node 
 * 
 * @post If there was enough free memory space, then a new Node was created  and its address was returned; otherwise, NULL was returned.
 * 
 * @returns address of created Node or NULL 
 */ 
Node* createNode( KType key, VType value, Node* left, Node* right ) {   // DONE
  Node* result = new Node;
  if (! result) return (Node*) NULL; 
  result->key = key;
  result->value = value;
  result->height = 0;
  result->left = left;
  result->right = right;
  return result;
}

/**
 * Recursively deletes the tree rooted at *root.
 *
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 * 
 * @pre (root == NULL), or *root is a valid Node
 * @post all Nodes in the tree that was rooted at *root have been deleted 
 * @post (root == NULL)
 */ 
void deleteTree( Node*& root ) {
  if ( root != NULL ) {
    deleteTree( root->left );
    deleteTree( root->right );
    delete root;
    root = NULL;
  }
}

/** 
 * Get node->height 
 * 
 * If (node == NULL) return -1 (the height of an empty tree). Otherwise return root->height. 
 * 
 * @param root = a Node pointer
 * @pre (root == NULL) or *root is a valid Node 
 * @returns (root) ? (root->height) : -1;
 */
int getHeight( Node * root ) {
  return (root) ? (root->height) : -1; 
}

/** 
 * Calculates the height of *node
 *
 * @param node = a Node pointer
 * @pre (node == NULL) or *node is a valid Node 
 * @returns 1 + maximum (height of left child, height of right child).
 */ 
int calcHeight( Node * node ) {
  if (! node) return -1; 
  int left_height = getHeight( node->left );
  int right_height = getHeight( node->right ); 
  return ( left_height > right_height ) ? left_height + 1 : right_height + 1; 
}

/** 
 * Sets the height of *node to the given value.
 * 
 * <B>Only updates root->height if the old value does not match</B> the calculated value.
 * 
 * @param node = a Node pointer 
 * @param height = the value to assign to node->height 
 * 
 * @pre (node == NULL) or *node is a valid Node 
 * @post node->height == height
 * 
 */ 
void setHeight( Node * node, int height ) {
  if (! node ) return;
  if (node->height == height) return; 
  node->height = height; 
}

/** Get node->value 
 * 
 * @param node = a Node pointer
 * @pre (node != NULL) && *node is a valid Node 
 * @returns node->value
 */
 VType getValue(Node* node) {
    assert( node ); 
    return node->value; 
} 
/** Set node->value 
 * 
 * @param node = a Node pointer
 * @param value = the new node->value
 * @pre (node != NULL) && *node is a valid Node 
 * @pre value is a valid VType 
 * @returns node->value(root)
 */
 void setValue(Node* node, VType value) {
    assert( node ); 
    node->value = value; 
 } 

/** 
 * "Rotates" the subtree <B>to the left (counter-clockwise)</B>.
 * 
 *  Since this is a counter-clockwise rotation, the left subtree will be increased and the right will be reduced. 
 *
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 *
 * @pre (root !=  NULL) && (root->right != NULL)
 * 
 * Three conditions must be true for rotateLeft(Node*&) to be the appropriate rotation to rebalance a Node. 
 * There are several equivalent ways of stating the needed condiditons, the ones that follow are not the one-and-only way.
 *
 *  <UL><LI> abs( getHeight( root->right ) - (getHeight( root->left ) ) GT 1  
 *       <UL><LI> which simply means that *root is unbalanced  </LI></UL>
 *  </LI><LI> getHeight( root->right ) GT getHeight( root->left )  
 *       <UL><LI> which means that the subtree rooted at root->right is too big </LI></UL>
 *  </LI><LI> and --EITHER--  
 *  </LI><LI> getHeight( root->right->right ) GT getHeight( root->right->left ) 
 *  </LI><LI> --OR-- 
 *  </LI><LI> getHeight( root->right )  NE  (1 + getHeight( root->right->left ))
 *  </LI><LI> --OR-- 
 *  </LI><LI> getHeight( root->right )  EQ  (1 + getHeight( root->right->right ))
 *       <UL><LI> which all mean that root->right->right has a greater height than root->right->left  </LI></UL>
 *  </LI></UL>
 * 
 * @post the new *root is the former right child of the old *root; the new left child of the new *root is the old *root 
 * @post the former left child of the new *root is now the new right child of the old *root 
 * 
 * @post the subtree that was rooted at *root has been rotated counter-clockwise 
 * @post getHeight( root ) == calcHeight( root ) for both the old and the new root nodes
 * 
 * @see The relevant Lecture slides for  <a href="ajh_slides/alan-hu-lecture09-avl-1up-page14-rotateLeft.pdf"><B>rotateLeft are HERE</B></a>.  
 * 
 */ 
void rotateLeft( Node *& root ) {
  assert(root != NULL); 
  assert(root->right != NULL);
  Node * temp = root->right;
  root->right = temp->left;
  temp->left = root;
  setHeight( root, calcHeight(root) );
  setHeight( temp, calcHeight(temp) );
  root = temp;
}

/** 
 * "Rotates" the subtree <B>to the right (clockwise)</B>.
 *
 *  Since this is a clockwise rotation, the left subtree will be reduced and the right increased. 
 * 
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 *
 * @pre (root != NULL) && (root->left != NULL)
 * 
 * Three conditions must be true for rotateRight(Node*&) to be the appropriate rotation to rebalance a Node. 
 *  Again, there are several equivalent ways of stating the needed condiditons. As long as the correct 
 *  rotation is chosen, the way the conditions are stated does not matter all that much. 
 *  
 *   <UL><LI> abs( getHeight( root->left ) - getHeight( root->right ) ) GT 1 
 *       <UL><LI> which simply means that *root is unbalanced  </LI></UL>
 *  </LI><LI> getHeight( root->left ) GT getHeight( root->right )  ---- which means that the subtree rooted at root->left is too big  
 *       <UL><LI> which means that the subtree rooted at root->left is too big </LI></UL>
 *  </LI><LI> and --EITHER--   
 *  </LI><LI> getHeight( root->left->left ) GT getHeight( root->left->right ) 
 *  </LI><LI> ---OR--- 
 *  </LI><LI> getHeight( root->left )  NE  (1 + getHeight( root->left->right )) 
 *  </LI><LI> ---OR--- 
 *  </LI><LI> getHeight( root->left )  EQ  (1 + getHeight( root->left->left )) 
 *       <UL><LI> which all mean that root->left->left has a greater height than root->left->right </LI></UL>
 *  </LI></UL>
 * 
 * @post the new *root is the former left child of the old *root, and the new right child of the new *root is the old *root 
 * @post the former right child of the new *root is now the new left child of the old *root 
 * 
 * @post the subtree that was rooted at *root has been rotated clockwise 
 * @post getHeight( root ) == calcHeight( root ) for both the old and the new root nodes
 *
 * @see The relevant lecture slides for  <a href="ajh_slides/alan-hu-lecture09-avl-1up-page11-rotateRight.pdf"><B>rotateRight are HERE</B></a>.
 * 
 */  
void rotateRight( Node *& root ) {
  assert(root != NULL); 
  assert(root->left != NULL); 
  Node * temp = root->left;
  root->left = temp->right;
  temp->right = root;
  setHeight( root, calcHeight(root) );
  setHeight( temp, calcHeight(temp) );
  root = temp;
}

/** 
 * doubleRotateLeft(root) is <B>rotateRight(root->right) then rotateLeft(root)</B>. 
 * 
 * First do rotateRight(root->right), a clockwise rotation on the right child of *root, to <B>deal with the root->right->left subtree</B> 
 * (the "big one in the middle"), and then rotateLeft(root), a counter-clockwise rotation on root.
 *
 * Three conditions must be true for doubleRotateLeft to be the appropriate procedure to rebalance the node. 
 * 
 *  <UL><LI> abs( getHeight( root->right ) - (getHeight( root->left ) ) GT 1 
 *       <UL><LI> which simply means that *root is unbalanced  </LI></UL>
 * </LI><LI> getHeight( root->right ) GT getHeight( root->left )  
 *       <UL><LI> which means that the subtree rooted at root->right is too big </LI></UL>
 *  </LI><LI> and --EITHER--   
 *  </LI><LI> getHeight( root->right->right ) LE getHeight( root->right->left ) 
 *  </LI><LI> ---OR--- 
 *  </LI><LI> getHeight( root->right )  EQ  (1 + getHeight( root->right->left ))
 *  </LI><LI> ---OR--- 
 *  </LI><LI> getHeight( root->right )  NE  (1 + getHeight( root->right->right ))
 *       <UL><LI> which all mean that root->right->left has a greater height than root->right->right  
 *      </LI><LI> so the subtree in the "middle" is the problem, and requires a pre-rotation 
 *      </LI></UL>
 *  </LI></UL>
 * 
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 * 
 * @pre (root != NULL) &&  (root->right != NULL) &&  (root->right->left != NULL)  
 *
 * @post the new *root is the former right->left grandchild of the old *root, and the new left child of the new *root is the old *root 
 * @post balance has been restored 
 * 
 * @see The relevant Lecture slides for <a href="ajh_slides/alan-hu-lecture09-avl-1up-page25-26-doubleRotateLeft.pdf"><B>doubleRotateLeft are HERE</B></a>.
 *   
 */ 
void doubleRotateLeft( Node *& root ) {
  assert(root != NULL);
  assert(root->right != NULL); 
  assert(root->right->left != NULL); 
  rotateRight(root->right);
  rotateLeft(root);
}

/** 
 * 
 * doubleRotateRight(root) is <B>rotateLeft(root->left) then rotateRight(root)</B>.
 *
 * First rotateLeft(root->left) -- do a counter-clockwise rotation on the left child of *root, to <B>deal with the root->left->right subtree</B> 
 * (the "big one in the middle"), and then rotateRight(root), a clockwise rotation on root itself. 
 *  
 * Three conditions must be true for doubleRotateRight to be the appropriate procedure to rebalance a Node. 
 * 
 *  <UL><LI> abs( getHeight( root->left ) - (getHeight( root->right ) ) GT 1 
 *       <UL><LI> which simply means that *root is unbalanced  </LI></UL>
 *  </LI><LI> getHeight( root->left ) GT getHeight( root->right ) 
 *       <UL><LI> which means that the subtree rooted at root->right is too big </LI></UL>
 *  </LI><LI> and --EITHER--   
 *  </LI><LI> getHeight( root->left->left ) LE getHeight( root->left->right ) 
 *  </LI><LI> ---OR--- 
 *  </LI><LI> getHeight( root->left )  EQ  (1 + getHeight( root->left->right )) 
 *  </LI><LI> ---OR--- 
 *  </LI><LI> getHeight( root->left )  NE  (1 + getHeight( root->left->left )) 
 *       <UL><LI> which all mean that root->left->right has a greater height than root->left->left 
 *      </LI><LI> so the subtree in the "middle" is the problem, and requires a pre-rotation 
 *      </LI></UL>
 *  </LI></UL> 
 * 
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 *
 * @pre (root != NULL) && (root->left != NULL) &&  (root->left->right != NULL)
 *  
 * @post the new *root is the former left->right grandchild of the old *root, and the new right child of the new *root is the old *root 
 * 
 * @see The relevant Lecture slides for <a href="ajh_slides/alan-hu-lecture09-avl-1up-page19-22-doubleRotateRight.pdf"> <B>doubleRotateRight are HERE</B></a>.  
 *
 */ 
void doubleRotateRight( Node *& root ) {
  assert(root != NULL);
  assert(root->left != NULL); 
  assert(root->left->right != NULL); 
  rotateLeft(root->left);
  rotateRight(root);
}

/** 
 * Ensure the AVL tree (or subtree) rooted at *root is balanced.
 *  
 * <B>Check if *root is unbalanced</B> (i.e. if the heights of its two children differ by more than one).  
 * If it is, then rebalance at *root. 
 * 
 * <UL><LI> IF getHeight( root->right ) GT getHeight( root->left ) THEN 
 *     <UL><LI> IF getHeight( root->right ) EQ (1 + getHeight( root->right->left )) THEN 
 *         <UL><LI> doubleRotateLeft(Node*&) </LI></UL>
 *     </LI><LI> ELSE   
 *         <UL><LI> rotateLeft(Node*&) </LI></UL>
 *     </LI></UL> 
 * </LI><LI> ELSE  
 *     <UL><LI> IF getHeight( root->left ) EQ (1 + getHeight( root->left->right )) THEN 
 *         <UL><LI> doubleRotateRight(Node*&) </LI></UL>
 *     </LI><LI> ELSE
 *         <UL><LI> rotateRight(Node*&) </LI></UL>
 *     </LI></UL> 
 * </LI></UL>
 * 
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 * 
 * @pre (root == NULL) or *root is a valid Node 
 * @pre balance(Node*&) is only called from insert(KType,VType,Node*&), and is only called if the value of root->height has been updated 
 * @pre *root may or may not be unbalanced 
 * 
 * @post (abs( getHeight( root->left ) - getHeight( root->right ) ) LE 1 ) ---- which just means that *root is balanced  
 */ 
void balance( Node *& root ) {
   if (! root) return; // base-case
 
   int left_height = getHeight( root->left );
   int right_height = getHeight( root->right );
   
   // if *root is not unbalanced, we're done 
   if (abs(right_height - left_height) <= 1) return; 

   if (right_height > left_height) {  
      assert( right_height > 1 + left_height );  // asserts are often used as a kind of documentation 
      
      if (root->right      && root->right->left  // always check that it's OKAY to dereference the pointers
      && (right_height == 1 + root->right->left->height)){
         doubleRotateLeft( root ); 
      }
      else { 
         rotateLeft( root ); 
      }
     
   }
   else { 
      assert( left_height > right_height + 1 ); 
           
      if (root->left      && root->left->right
      && (left_height == 1 + root->left->right->height)){
         doubleRotateRight( root );  
      }
      else { 
         rotateRight( root ); 
      }
   } 
}

/**
 *  <B>Insert a new Node,</B> with the given key and value, into the AVL tree rooted at *root, restoring balance if needed.
 * 
 *  If (root == NULL) then the tree (or subtree) V empty, so a new Node is created and it becomes the root of the tree or subtree. 
 *  Otherwise, a standard Binary Search is used to attempt to find an existing Node with the given key value. 
 * 
 *  Since an AVL tree is a Binary Search Tree there cannot be two Nodes with the same key. But if a Node with the given key already exists 
 *  no error or warning message is printed. Processing simply continues, as if the Node had just been added. 
 * 
 *  If no Node with the given key already exists in the tree, then one is created and it becomes the left or right child of an existing Node. 
 *  <B>Note that the recursive calls use a reference to the left or right pointer of an existing Node, so when "root = createNode(key)" executes, 
 *  the left or right pointer of the parent is updated.</B> On the other hand, if "createNode(key)" is executed on the original call (and not during a 
 *  recursive call) then the root of the tree itself is updated.  
 *   
 *  @param key = the key of the new Node
 *  @param value = the value of the new Node
 *  @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 * 
 *  @pre (root == NULL), or *root is a valid Node 
 *  @pre if (root == NULL) then the tree (or subtree) rooted at *root is empty, and the new Node will be the become the *root of the new subtree. 
 * 
 *  @post a Node with the given key exists in the tree rooted at *root
 *  @post the Node itself, and all its ancestors, have had their heights updated (if necessary) and any required rebalancing has been done
 * 
 */
void insert( KType key, VType value, Node *& root ) {
  if( root == NULL ) {  // base case 
    root = createNode(key, value);
    return;
  }  
  else if( key < root->key ) {
    insert( key, value, root->left );
  }
  else if( key > root->key ) {                       
    insert( key, value, root->right );
  }
  // update root->height if it's changed, and balance(root) 
  int calcHeight_root = calcHeight( root );; 
  if( calcHeight_root != getHeight( root) ) {
    setHeight( root, calcHeight_root ); 
    balance(root);
  }
}

/** 
 * Return a pointer reference to the Node in the tree with the given key.
 *
 * If a Node with the given key exists in the tree, a pointer reference to it is returned. 
 * Otherwise a reference to the pointer where it would be inserted is returned.  
 * NOTE in the latter case, the address contained in the pointer is NULL. 
 *    
 * @param key = the key value to locate
 * @param root = a reference to the Node pointer that contains the address of the root of the tree (or subtree)
 * @pre (root == NULL) or *root is a valid Node
 * @returns a pointer reference to the Node with the given key, or a useful NULL if not found 
 */ 
Node*& findNode(KType key, Node*& root) { 
    if (! root) { 
        return root; 
    }
    if (key < root->key) { 
        return findNode( key, root->left); 
    }
    if (key > root->key) { 
        return findNode( key, root->right);
    }  
    return root; 
}

/** 
 * Return true iff the given key value is the key of a Node in the tree rooted at *root. 
 * 
 * @param key = the key value to locate
 * @param root = a Node pointer that contains the address of the root of the tree (or subtree)
 *
 * @pre (root == NULL) or *root is a valid Node 
 * @returns true if and only if there is a Node in the tree rooted at *root with key data member equal to the given key value
 */
bool hasKey( KType key, Node * root ) {
  if (! root) {
    return false;
  }
  else if ( key < root->key ) {
    return hasKey( key, root->left );
  }
  else if ( key > root->key ) {
    return hasKey( key, root->right );
  }
  return true;
}

/** 
 * Return a reference to the value associated with the key in the tree
 * 
 * @param key = the key value to locate
 * @param root = a Node pointer that contains the address of the root of the tree (or subtree)
 *
 * @pre (root != NULL) && *root is a valid Node 
 * @pre hasKey( key, root ) is true 
 * 
 * @returns a reference to the value associated with the key in the tree
 */
VType & lookup( KType key, Node * root ) {
  assert ( root != NULL );

  if ( key < root->key ) {
    return lookup( key, root->left );
  }
  else if ( key > root->key ) {
    return lookup( key, root->right );
  }
  else
    return root->value;
}

/*
 * Helper to print the tree sideways. 
 */
void printTreeHelper( Node * root, int depth ) {
  if( root == NULL ) return;
  printTreeHelper( root->right, depth + 1 );
  std::cout << std::setw( 3 * depth ) << "";		/* output 3 * d spaces  */
  std::cout << root->key << std::endl;
  printTreeHelper( root->left, depth + 1 );
}
/**
 * Print out the tree sideways.
 * 
 * @param root = a Node pointer that contains the address of the root of the tree (or subtree)
 * 
 * @pre (root == NULL) or *root is a valid Node 
 * @post the tree rooted at *root is printed in reverse inorder with nodes  indented (3 * depth) spaces; 
 *     This produces a tree that has its root near  the left side of the screen and the leaves near the right side.
 */

void printTree( Node * root ) {
  printTreeHelper( root, 0 );
}

/**
 * Return true iff *root is the root of a valid AVL tree. 
 * @param root = a Node pointer that contains the address of the root of the tree (or subtree)
 * @returns true iff *root is the root of a valid AVL tree. 
 */
bool isValidAVLTree( Node* root ){ 
  if (! root) return true; 
  if (isValidAVLTree( root->left ) && isValidAVLTree( root->right )) {
    if (abs(getHeight( root->left ) - getHeight( root->right )) > 1) return false; 
    return true; 
  }
  else return false; 
}
int totalValue( Node* root ){ 
  if (! root) return 0; 
  return totalValue( root->left) + totalValue( root->right ) + root->value; 
}

/** 
 * Traverse the tree (visit all the nodes exactly once) sequentially in post-order 
 * to locate the word that was used the most. 
 * 
 * @pre count has been initialized to an appropriate default value,  
 *      meaning some valid VType value which is LESS THAN any stored in the tree
 * <UL><LI> for VType int, where every new Node begins with value = 1 
 *          and is only ever added to, 0 is good 
 * </LI></UL> 
 * 
 * @post count is the maximum value in the tree and 
 *      word is the key associated with that value
 * @returns nothing 
 */ 
void findMaxSequential(Node * root, KType & word, VType & count) {
  if (root == NULL) {
    return;
  }

  // Find the max in the left subtree.
  KType lword = word;  // intializing the string objects is the right thing to do  
  VType lmax = count;  // in general, and will make do difference in sequential/recursive
  KType rword = word;  // processing, but DO NOT INITIALIZE for OpenMP processing  
  VType rmax = count;
  findMaxSequential(root->left, lword, lmax);

  // Find the max in the right subtree.
  findMaxSequential(root->right, rword, rmax);

  // Record for return the largest of the left-max, right-max, and
  // current node's key/value.
  if (rmax > lmax && rmax > root->value) {
    word = rword;
    count = rmax;
  }
  else if (lmax > root->value) {
    word = lword;
    count = lmax;
  }
  else {
    word = root->key;
    count = root->value;
  }
}

/** 
 * Helper for findMax. 
 * 
 * Fork off tasks to handle subtrees until it is known for sure, by 
 * looking at the root node of a subtree that it has atmost 1100 nodes, 
 * at which point it should switch to the provided sequential version instead.
 *  
 */ 
void findMaxHelper(Node * root, KType & word, VType & count) {
  // 
  // READ THE findMax COMMENTS AND LOOK AT findMax CODE   
  //
  // It's nothing much.. but it's easy to forget when using OpenMP!

  // ************ TODO: Implement this! *************
    if (root->height < 5 ) {
        findMaxSequential(root, word, count);
        return;
    }
    KType lword;
    VType lmax = count;
    KType rword;
    VType rmax = count;
#pragma omp task untied shared(lword, lmax)
    // Find the max in the left subtree.
    findMaxHelper(root->left, lword, lmax);
    // Find the max in the right subtree.
    findMaxHelper(root->right, rword, rmax);
#pragma omp taskwait
    
    // Record for return the largest of the left-max, right-max, and
    // current node's key/value.
    if (rmax > lmax && rmax > root->value) {
        word = rword;
        count = rmax;
    }
    else if (lmax > root->value) {
        word = lword;
        count = lmax;
    }
    else {
        word = root->key;
        count = root->value;
    }
}



/** 
 * Traverse the tree (visit all the nodes exactly once) in post-order 
 * using divide-and-conquer, fork/join-style parallelism, 
 * to locate the word that was used the most.
 * 
 * @pre word has been DECLARED as a KType variable BUT NOT INITIALIZED
 * 
 * @pre count has been DECLARED AND INTIALIZED initialized to an appropriate default value,  
 *      meaning some valid VType value which is LESS THAN any stored in the tree
 * <UL><LI> for VType int, where every new Node begins with value = 1 
 *          and is only ever added to, 0 is good 
 * </LI></UL> 
 *
 * @post count is the maximum value in the tree and 
 *      word is the key associated with that value
 * @returns nothing 
 */ 
void findMax(Node * root, KType & word, VType & count) {
   // "Spin up" parallelism for OpenMP.
#pragma omp parallel
#pragma omp single
  findMaxHelper(root, word, count);
}

/**
 * Print a message on stdout about how to use this program. 
 * @param program = the argv[0] value == the name used to invoke this rununit
 */ 
void usage(const char* program) {
  std::cout << "Usage: " << program << " <filename> [cutoff]"  << std::endl;
  std::cout << "\tPrints the most common word (with its count) in the named file."
       << std::endl;
}

/** 
 * Runs REPETITIONS trials of two different types of searching the tree. 
 * 
 * @param f = a reference to a function that returns nothing and takes the given parameter types 
 * @param tree = the root of an AVL tree 
 * @param k = a reference to a valid KType variable (which has been DECLARED but not initialized)  
 * @param v = a reference to a valid VType variable (which has been appropriately initialized)  
 * @param type = a description to use when printing the results 
 */ 
void findMostCommonAndReport(std::function<void(Node*, KType&, VType&)> f,
			     Node * tree, KType & k, VType & v, 
			     const char * type) {
  KType kdefault = k;
  VType vdefault = v;

  Timer timer;
  for (int i = 0; i < REPETITIONS; i++) {
    KType knew = kdefault;
    VType vnew = vdefault;
    f(tree, knew, vnew);
    assert(i == 0 || (knew == k && vnew == v));
    k = knew;
    v = vnew;
  }
  auto time = timer.timeSoFar();
  std::chrono::duration<double> timeS = time;
  std::cout << type << "\t" << k << "\t" << v << "\t"
	    << timeS.count() << std::endl;
  std::cout.flush();  
}

/**
 * Every CPP rununit has one, and only one, main(int, char**) function.  
 */
int main( int argc, char *argv[] ) {
  if (argc < 2 || argc > 3) {
    usage(argv[0]);
    return -1;
  }
  if (argc == 3) { 
    SEQUENTIAL_CUTOFF = std::atoi( argv[2] );
  } 

  std::fstream infile; 
  infile.open( argv[1], std::fstream::in ); 
  if (infile.rdstate()) { // some type of error 
    std::cout<< "Error opening " << argv[1] << std::endl; 
    if ((infile.rdstate() &  std::ifstream::failbit) != 0 ) std::cout << " failbit";  
    if ((infile.rdstate() &  std::ifstream::badbit) != 0 )  std::cout << " badbit";  
    if ((infile.rdstate() &  std::ifstream::eofbit) != 0 )  std::cout << " eofbit"; 
    std::cout << std::endl;  
    exit(0); 
  } 

  // Tally all the words.
  Node *avlTree_root = NULL;
  std::string word;
  
  // foreach word, attempt to find a Node (with word as its key) already in tree 
  // if none exists, then make one, with value == 0
  // always increment the tally for the word by 1  

  while (infile >> word) {
      Node* node = findNode( word, avlTree_root ); 
      if (! node) {                  
          insert( word, 1, avlTree_root );   
      } 
      else setValue( node, 1 + getValue(node) ); 
  }

  // Find the most common word, first with parallel processing then sequential
  std::cout << "cutoff = " << SEQUENTIAL_CUTOFF << std::endl; 
  
  int count = 0;  
  word = "";
  findMostCommonAndReport(&findMax, avlTree_root, word, count, "PARALLEL");

  count = 0;
  word = "";
  findMostCommonAndReport(&findMaxSequential, avlTree_root, word, count, "SEQUENTIAL");

  // Free up allocated memory
  deleteTree(avlTree_root);

  return 0;
}
