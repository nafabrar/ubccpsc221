//! @file avl.cc 
//! To complete cs221 2014W2 lab8, implement balance(Node*&).
//! 
//! You'll notice there are a LOT more comments than usual in this file. The 
//! comments (including this paragraph) became HTML documentation (see the 
//! link on the Course webpage > "Labs/Lecture Notes" > "Lab 8" > "March 12"). 
//! The comments are mostly at the top of each procedure, along with some extra
//! code needed by the Unit testing framework, and some "trace" prints that 
//! you can temporarily uncomment, so you can see what procedures are being 
//! executed as each key is inserted into the tree. The actual crucial program 
//! code is relatively comment free, so it stands out from the rest of the source. 
//! 
//! To insert the defaultKeys into the AVL tree, print the tree after each 
//! insertion, have the the Unit test framework verify the results as you go, 
//! and (hopefully) print the same trees as in the supplied output.txt file,  
//! just write the balance(Node*&) function, and then compile and run your 
//! program with no command line arguments.                          @code 
//!       make 
//!       ./avl                                                      @endcode
//!                                                                  
//! Or: build a different AVL_tree using your own keys. Just enter them as 
//! command line arguments. The tree will be drawn after each insertion, but 
//! the test framework cannot check the correctness of those trees.  @code 
//!     make 
//!     ./avl 6 49 5 15 30 12 14 19                                  @endcode 
                                                                  
#include <iostream> // cout, endl   
#include <sstream>  // stringstream 
#include <iomanip>	// setw()       
#include <cstdlib>	// atoi() abs() 
#include <cassert>  // assert()     
#include <vector>   // vector       
#include <string>   // string       
#include "Unit.h" 
/* *************************************************************************** */ 
/* ********** DO NOT CHANGE ANYTHING EXCEPT: ********************************* */
/* ********** void balance( Node *& root )   WRITE THIS METHOD (IT COMES RIGHT */ 
/* ***************************************** AFTER THE FUNCTION PROTOTYPES)    */
/* *************************************************************************** */ 
/* **** You can also temporarily uncomment lines like the following to ******* */
/* **** print out a "trace" of the program execution. To help you quickly **** */
/* **** find these lines, they are all preceded by a comment line like: ****** */
// ---------------------------------- trace ---------------------------------- 
// std::cout << "rotateLeft: root->key = " << root->key << std::endl;
// ---------------------------------------------------------------------------
/* *************************************************************************** */

/** 
 * @typedef int KType 
 * the datatype for the key of a Node
 */
typedef int KType;

/**
 * @struct Node 
 * A Node in an AVL tree.
 * AVL trees are self-balancing Binary Search Trees, and are named after their 
 * inventors, Georgy Adelson-Velsky, and E. M. Landis, who co-authored 
 * "An algorithm for the organization of information" in 1962. 
 * <a href="http://en.wikipedia.org/wiki/AVL_tree"> wiki_AVL_tree </a> 
 *
 * The height of each Node is stored as a data-member, and calculated as:
 * @code  
 * 1 + MAX( height of left child, height of right child).  
 * @endcode
 * as required. 
 */
struct Node {
  KType key;     /**< the key of the node */   
  int height;    /**< the height of the node */
  Node * left;   /**< pointer to the left child of the node (or NULL) */
  Node * right;  /**< pointer to the right child of the node (or NULL) */
};

/* function prototypes */
void insert( KType key, Node *& root );
void balance( Node *& root );
void rotateLeft( Node *& root );
void rotateRight( Node *& root );
void doubleRotateLeft( Node *& root );
void doubleRotateRight( Node *& root );
bool updateHeight( Node * root );
int height( Node * root );
bool contains( KType key, Node * root );
void prepareExpected(int num_to_skip, int num_to_take, std::vector<int>& expected); 
Node* createNode( KType key, Node* left = NULL, Node* right = NULL );
Node* findNode(KType key, Node* root); 
void printTreeHelper( Node * root, int depth );
void printTree( Node * root );
void visitTreeHelper( Node* root, int level, std::vector<int>& visitor);
void visitTree(Node* root, std::vector<int>& visitor); 
void deleteTree( Node*& root );

/* ********** DO NOT CHANGE ABOVE HERE **************** */

/** 
 * Rebalance the AVL at *root if necessary.
 *  
 * <B>Check if *root is unbalanced</B> (i.e. if the heights of its two children 
 * differ by more than one).  If it is, rebalance at root using whichever 
 * one of the following is appropriate. 
 *  
 * @li rotateLeft(Node*&)
 * @li rotateRight(Node*&) 
 * @li doubleRotateLeft(Node*&) 
 * @li doubleRotateRight(Node*&) 
 *
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * 
 * @pre root is NULL, or *root is a valid Node 
 * @pre balance(Node*&) is only called from insert(KType, Node*&), 
 *      and is not called unless root->height has been updated 
 * @pre *root may or may not be unbalanced 
 * 
 * @post abs( height( root->left ) - height( root->right ) ) LE 1 
 *      ---- which means that *root is balanced (by definition)  
 * @returns nothing
 */ 
void balance( Node *& x ) {
    //
    // Check if node x is unbalanced (i.e., the heights of its
    // two children differ by more than one).  If it is, rebalance
    // at x using one of rotateLeft, rotateRight, doubleRotateLeft,
    // or doubleRotateRight, whichever is appropriate.
    //
    int left_height,left_left_height,left_right_height,right_left_height,right_height,right_right_height;
    
    if(x->left == NULL){
        left_height = -1;
    }else{
        left_height = x->left->height;
        left_left_height = (x->left->left == NULL) ?  -1: x->left->left->height;
        left_right_height = (x->left->right == NULL) ?  -1: x->left->right->height;
    }
    if(x->right == NULL){
        right_height = -1;
    }else{
        right_height = x->right->height;
        right_left_height = (x->right->left == NULL) ?  -1: x->right->left->height;
        right_right_height = (x->right->right == NULL) ?  -1: x->right->right->height;
    }
    
    if(left_height - right_height != 0 and
       left_height - right_height != -1 and
       left_height - right_height != 1){
        
        if(left_height - right_height < 0){ // Rotate-Left
            if(right_left_height - right_right_height < 0){
                rotateLeft(x);
                // cout<<"(rotateLeft)"<<endl;
            }else{
                doubleRotateLeft(x);
                // cout<<"(doubleRotateLeft)"<<endl;
            }
        }else{ // Rotate-Right
            if(left_left_height - left_right_height > 0){
                rotateRight(x);
                // cout<<"(rotateRight)"<<endl;
            }else{
                doubleRotateRight(x);
                // cout<<"(doubleRotateRight)"<<endl;
            }
        }
        
    }}
    
    





/* ********** DO NOT CHANGE BELOW HERE **************** */


Unit unit;                 /**< An instance of the Unit testing framework. */ 
std::vector<int> visitor;  /**< vector for Unit testing tree traversal */ 
std::vector<int> expected; /**< vector to hold expected values for Unit testing*/ 

/** 
 * "Rotates" the subtree <B>to the left (counter-clockwise)</B>.
 * 
 *  Since this is a counter-clockwise rotation, 
 *  the left subtree will be increased and the right will be reduced. 
 *
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 *
 * @pre root is not NULL
 * @pre root->right is not NULL 
 * 
 * @pre Three conditions must be true for rotateLeft(Node*&) to be the appropriate
 *  rotation to rebalance a Node. 
 *  <UL>
 *  <LI> There are several equivalent ways of stating the needed condiditons, 
 *       the ones that follow are not the one-and-only way.)
 *  <LI> abs( height( root->right ) - (height( root->left ) ) GT 1 
 *     ---- which simply means that *root is unbalanced  
 *     ---- clearly only unbalanced Nodes should be rotated </LI>
 *  <LI> height( root->right ) GT height( root->left )  
 *     ---- which means that the subtree rooted at root->right is too big 
 *     (so counter-clockwise SEEMS appropriate) </LI>
 *    
 *  <LI> height( root->right->right ) GT height( SOMETHING ) ---OR--- </LI>
 *  <LI> height( root->right )  NE  (1 + height( SOMETHING-ELSE )) </LI>
 *  </UL>
 * 
 * @post the subtree that was rooted at *root has been rotated counter-clockwise 
 * @post height of the old *root has been updated (if necessary) 
 * @post height of the new *root has been updated (if necessary) 
 * @post balance has been restored
 * 
 * @post the new *root is the former right child of the old *root
 * @post the new left child of the new *root is the old *root 
 * @post the former left child of the new *root is now 
 *       the new right child of the old *root 
 * 
 * @see The relevant Lecture slides for  
 * <a href="ajh_slides/alan-hu-lecture09-avl-1up-page14-rotateLeft.pdf">
 * <B>rotateLeft are HERE</B></a>.  
 * 
 * @returns nothing 
 */ 
void rotateLeft( Node *& root ) {
  /* if Unit testing is active, use its insist method */
  /* so even if the assert() fails, the errors get reported first  */
  if (unit.isActive()) {
      unit.insistNonNull("rotateLeft Node* root", root); 
  }
  /* not wrapped in else-body (so: there is no doubt this assert happens) */
  assert(root != NULL); 
  /* ditto */ 
  if (unit.isActive()) {
      unit.insistNonNull("rotateLeft Node* root->right", root->right); 
  }
  assert(root->right != NULL);
  // ---------------------------------- trace ----------------------------------
  // std::cout << "rotateLeft: root->key = " << root->key << std::endl;
  // ---------------------------------------------------------------------------
  /* end of extra code for tests and traces; the crucial statements are next */ 
  /* ----------------------------------------------------------------------- */ 
  Node * temp = root->right;
  root->right = temp->left;
  temp->left = root;
  updateHeight(root);
  updateHeight(temp);
  root = temp;
}
/** 
 * "Rotates" the subtree <B>to the right (clockwise)</B>.
 *
 *  Since this is a clockwise rotation, 
 *  the left subtree will be reduced and the right increased. 
 * 
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 *
 * @pre root is not NULL
 * @pre root->left is not NULL
 * 
 * @pre Three conditions must be true for rotateRight(Node*&) to be the appropriate
 *  rotation to rebalance a Node. 
 *  <UL>
 *  <LI> Again, there are several equivalent ways of stating the needed condiditons. 
 *       As long as the correct rotation is chosen, the way the conditions are 
 *       stated does not matter all that much. </LI>
 *  <LI> abs( height( root->left ) - (height( root->right ) ) GT 1 
 *     ---- which simply means that *root is unbalanced  
 *     ---- clearly only unbalanced Nodes should be rotated </LI>
 *  <LI> height( root->left ) GT height( root->right )  
 *     ---- which means that the subtree rooted at root->left is too big  
 *     (so clockwise SEEMS appropriate) </LI>
 *    
 *  <LI> height( root->left->left ) GT height( ANOTHER-THING ) ---OR--- </LI>
 *  <LI> height( root->left )  NE  (1 + height( AND-YET-ANOTHER )) </LI>
 *  </UL>
 
 * @post the subtree that was rooted at *root has been rotated clockwise 
 * @post height of the old *root Node has been updated (if necessary) 
 * @post height of the new *root Node has been updated (if necessary) 
 * @post balance has been restored 
 * 
 * @post the new *root is the former left child of the old *root 
 * @post the new right child of the new *root is the old *root 
 * @post the former right child of the new *root is now 
 *       the new left child of the old *root 
 *
 * @see The relevant lecture slides for 
 * <a href="ajh_slides/alan-hu-lecture09-avl-1up-page11-rotateRight.pdf">
 * <B>rotateRight are HERE</B></a>.
 * 
 * @returns nothing 
 */ 
void rotateRight( Node *& root ) {
  /* if Unit testing is active, use its insist method */
  /* so even if the assert() fails, the errors get reported first  */
  if (unit.isActive()) {
      unit.insistNonNull("rotateRight Node* root", root); 
  }
  /* not wrapped in else-body (so: there is no doubt this assert happens) */
  assert(root != NULL); 
  /* ditto */
  if (unit.isActive()) { 
      unit.insistNonNull("rotateRight Node* root->left", root->left); 
  }
  assert(root->left != NULL); 
  // ---------------------------------- trace ----------------------------------
  // std::cout << "rotateRight: root->key = " << root->key << std::endl; 
  // ---------------------------------------------------------------------------
  /* end of extra code for tests and traces; the crucial statements are next */ 
  /* ----------------------------------------------------------------------- */ 
  Node * temp = root->left;
  root->left = temp->right;
  temp->right = root;
  updateHeight(root);
  updateHeight(temp);
  root = temp;
}

/** 
 * doubleRotateLeft(root) is <B>rotateRight(root->right) then rotateLeft(root)</B>. 
 * 
 * First do rotateRight(root->right), a clockwise rotation on the right child of assertT *root, to <B>deal with the root->right->left subtree</B> (the "big one in the 
 * middle"), and then rotateLeft(root), a counter-clockwise rotation on root.
 *  
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 *
 * @pre root is not NULL 
 * @pre root->right is not NULL 
 * @pre root->right->left is not NULL  
 *
 * @pre Three conditions must be true for doubleRotateLeft to be the appropriate
 *  procedure to rebalance a node. 
 *  <UL>
 *  <LI> abs( height( root->right ) - (height( root->left ) ) GT 1 
 *     ---- which simply means that *root is unbalanced  
 *     ---- only unbalanced Nodes should be rotated </LI>
 *  <LI> height( root->right ) GT height( root->left )  
 *     ---- which means that the subtree rooted at root->right is too big  </LI>
 *    
 *  <LI> height( root->right->right ) LE height(  SOMETHING  ) ---OR--- </LI>
 *  <LI> height( root->right )  EQ  (1 + height( SOMETHING-ELSE )) </LI>
 *  </UL>
 * 
 * @post the new *root is the former right->left grandchild of the old *root
 * @post the new left child of the new *root is the old *root 
 * @post balance has been restored 
 * 
 * @see The relevant Lecture slides for 
 * <a href="ajh_slides/alan-hu-lecture09-avl-1up-page25-26-doubleRotateLeft.pdf">
 * <B>doubleRotateLeft are HERE</B></a>.
 *   
 * @returns nothing 
 */ 
void doubleRotateLeft( Node *& root ) {
  /* if Unit testing is active, use its insist method */
  /* so even if the assert() fails, the errors get reported first  */
  if (unit.isActive()) { 
      unit.insistNonNull("doubleRotateLeft Node* root", root); 
  }
  /* not wrapped in else-body (so: there is no doubt this assert happens) */
  assert(root != NULL);
  /* ditto */
  if (unit.isActive()) { 
      unit.insistNonNull("doubleRotateLeft Node* root->right", root->right); 
  }
  assert(root->right != NULL); 
  /* ditto */
  if (unit.isActive()) { 
      unit.insistNonNull("doubleRotateLeft Node* root->right->left", root->right->left); 
  }
  assert(root->right->left != NULL); 
  // ---------------------------------- trace ----------------------------------
  // std::cout << "doubleRotateLeft: root->key = " << root->key << std::endl; 
  // ---------------------------------------------------------------------------
  /* end of extra code for tests and traces; the crucial statements are next */ 
  /* ----------------------------------------------------------------------- */ 
  rotateRight(root->right);
  rotateLeft(root);
}

/** 
 * 
 * doubleRotateRight(root) is <B>rotateLeft(root->left) then rotateRight(root)</B>.
 *
 * First do rotateLeft(root->left), a counter-clockwise rotation on the left 
 * child of *root, to <B>deal with the root->left->right subtree</B> (the "big one 
 * in the middle"), and then rotateRight(root), a clockwise rotation on root.
 *  
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 *
 * @pre root is not NULL 
 * @pre root->left is not NULL 
 * @pre root->left->right is not NULL
 *  
 * @pre Three conditions must be true for doubleRotateRight to be the appropriate
 * procedure to rebalance a Node. 
 * <UL>
 * <LI> abs( height( root->left ) - (height( root->right ) ) GT 1 
 *     ---- which simply means that *root is unbalanced  
 *     ---- only unbalanced Nodes should be rotated </LI>
 * <LI> height( root->left ) GT height( root->right )  
 *     ---- which means that the subtree rooted at root->left is too big  </LI>
 *    
 * <LI> height( root->left->left ) LE height( ANOTHER-THING ) ---OR--- </LI>
 * <LI> height( root->left )  EQ  (1 + height( AND-YET-ANOTHER )) </LI>
 * </UL>
 * 
 * @post the new *root is the former left->right grandchild of the old *root
 * @post the new right child of the new *root is the old *root 
 * @post balance has been restored 
 * 
 * @see The relevant Lecture slides for 
 * <a href="ajh_slides/alan-hu-lecture09-avl-1up-page19-22-doubleRotateRight.pdf">
 * <B>doubleRotateRight are HERE</B></a>.  
 *
 * @returns nothing 
 */ 
void doubleRotateRight( Node *& root ) {
  /* if Unit testing is active, use its insist method */
  /* so even if the assert() fails, the errors get reported first  */
  if (unit.isActive()) {
      unit.insistNonNull("doubleRotateRight Node * root", root); 
  }
  /* not wrapped in else-body (so: there is no doubt this assert happens) */
  assert(root != NULL);
  /* ditto */
  if (unit.isActive()) { 
      unit.insistNonNull("doubleRotateLeft Node* root->left", root->left); 
  }
  assert(root->left != NULL); 
  /* ditto */
  if (unit.isActive()) { 
      unit.insistNonNull("doubleRotateLeft Node* root->left->right", root->left->right); 
  }
  assert(root->left->right != NULL); 
  // ---------------------------------- trace ----------------------------------
  // std::cout << "doubleRotateRight: root->key = " << root->key << std::endl; 
  // ---------------------------------------------------------------------------
  /* end of extra code for tests and traces; the crucial statements are next */ 
  /* ----------------------------------------------------------------------- */ 
  rotateLeft(root->left);
  rotateRight(root);
}

/**
 * Creates a new Node with the given key. 
 * 
 * Its left child will be 'left', its right child 'right',  
 * and its height = 0. 
 *
 * @param key = the value for the key of the new Node
 * @param left = the address of the Node that will be the 
 *        left child of the new Node, or NULL
 * @param right = the address of the Node that will be the 
 *        right child of the new Node, or NULL
 * 
 * @pre  key is a valid KType 
 * @pre  left is a Node pointer, or is NULL, or omitted (defaults to NULL)
 * @pre  right is a Node pointer, or is NULL, or omitted (defaults to NULL) 
 * @post If there was enough free memory space, then a new Node was created  
 *       and its address was returned; otherwise, NULL was returned.
 * @returns address of created Node 
 */ 
Node* createNode( KType key, Node* left, Node* right) {
  Node* result = new Node;
  // ---------------------------------- trace ----------------------------------
  // std::cout << "createNode( " << key << " )" << std::endl;
  // ---------------------------------------------------------------------------
  if (! result) return (Node*) NULL; 
  result->key = key;
  result->height = 0;
  result->left = left;
  result->right = right;
  return result;
}

/**
 * Recursively deletes the tree rooted at *root.
 *
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * 
 * @pre root is NULL, or *root is a valid Node
 * @post all Nodes in the tree that was rooted at *root have been deleted 
 * @post root is NULL
 * @returns nothing  
 */ 
void deleteTree( Node*& root ) {
  if ( root != NULL ) {
    // ---------------------------------- trace ----------------------------------
    // std::cout << "deleteTree( " << root->key << " )" << std::endl;
    // ---------------------------------------------------------------------------
    deleteTree( root->left );
    deleteTree( root->right );
    delete root;
    root = NULL;
  }
}

/** 
 * Returns the height of *root. 
 * If root is NULL, return -1 (the height of an empty tree). 
 * Otherwise return root->height. 
 * 
 * @param root = a Node pointer
 * 
 * @pre root is NULL, or *root is a valid Node 
 * @returns (root == NULL ? -1 : root->height )
 */
int height( Node * root ) {
  if( root == NULL ) return -1;
  return root->height;
}

/** 
 * Recalculates the height of *root (and updates root->height if necessary).  
 * 
 * Ensures root->height = 1 + maximum (height of left child, height of right child).
 * <B>Only updates root->height if the old value does not match</B> the calculated value.
 * 
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * 
 * @pre root is NULL, or *root is a valid Node 
 * @post root->height has been updated with the new value (if necessary) 
 * @returns true iff the value of root->height has changed   
 */ 
bool updateHeight( Node * root ) {
  if( root == NULL ) return false;
  int m = std::max(height(root->left), height(root->right)) + 1;
  if( root->height != m ) {
    // ---------------------------------- trace ----------------------------------
    // std::cout << "updateHeight( " << root->key << " ) "; 
    // std::cout << "from " << root->height << " to " << m << std::endl; 
    // ---------------------------------------------------------------------------
    root->height = m;
    return true;
  }
  // ---------------------------------- trace ----------------------------------
  // std::cout << "updateHeight( " << root->key << " ) no change" << std::endl; 
  // ---------------------------------------------------------------------------
  return false;
}

/** 
 * Return true iff the given key value is the key of a Node 
 * in the tree rooted at *root. 
 * 
 * @param key = the key value to locate
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 *
 * @pre root is NULL, or *root is a valid Node 
 * @returns true if and only if there is a Node in the tree rooted at *root 
 *          with key data member equal to the given key value
 */
bool contains( KType key, Node * root ) {
  if (! root) {
    return false;
  }
  else if ( key < root->key ) {
    return contains( key, root->left );
  }
  else if ( key > root->key ) {
    return contains( key, root->right );
  }
  return true;
}


/**
 *  <B>Insert a new Node,</B> with the given key value, into the AVL tree 
 *  rooted at *root, restoring balance if needed.
 * 
 *  If root is NULL, then the existing tree (or subtree) is empty, 
 *  so a new Node is created and it becomes the root of the tree (or subtree). 
 *  Otherwise, the standard Binary Search Tree procedure is used to 
 *  determine if a Node with the given key already exists in the tree. 
 * 
 *  Since an AVL tree is a Binary Search Tree there can be no duplicates, 
 *  but if a Node with the given key already exists the procedure just 
 *  bypasses the createNode(key) portion of the insert algorithm. 
 *  No error message is printed; the procedure does not complain in any way.  
 *  Processing continues as if a Node was added (the heights of that Node 
 *  and its ancestors are checked as the call-stack unwinds from the 
 *  recursive calls). 
 * 
 *  If no Node with the given key already exists in the tree, then 
 *  one is created and it becomes the left or right child of an existing Node. 
 *  <B>Note that the recursive calls use a reference to the left or right 
 *  pointer of an existing Node, so when "root = createNode(key)" executes, 
 *  the left or right pointer of the parent is updated.</B> On the other hand, 
 *  if "createNode(key)" is executed on the original call (and not during a 
 *  recursive call) then the root of the tree itself is updated.  
 *     
 * @param key = the value for the key of the new Node
 * @param root = a reference to the Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * 
 * @pre root is NULL, or *root is a valid Node 
 * @pre if root is NULL then the AVL tree is empty, and the new Node will 
 *      be the new *root  
 * @post a Node with the given key exists in the tree rooted at *root
 * @post the Node itself, and all its ancestors, have had their heights 
 *  updated (if necessary) and any required rebalancing has been done
 * @returns nothing 
 * 
 */
void insert( KType key, Node *& root ) {
  /* BASE CASE */
  if( root == NULL ) {
    // ---------------------------------- trace ----------------------------------
    // std::cout << "insert( " << key << " ) creating new Node" << std::endl; 
    // ---------------------------------------------------------------------------
    root = createNode(key);
    return;
  }
  /* either go to the left, or to the right, but ignore if already in tree */
  if( key < root->key ) {
    // ---------------------------------- trace ----------------------------------
    // std::cout << "insert( " << key << " ) recursing on left child since ";
    // std::cout << "root->key = " << root->key << std::endl; 
    // ---------------------------------------------------------------------------
    insert( key, root->left );
  }
  else if( key > root->key ) {                       
    // ---------------------------------- trace ----------------------------------
    // std::cout << "insert( " << key << " ) recursing on right child since ";
    // std::cout << "root->key = " << root->key << std::endl; 
    // ---------------------------------------------------------------------------
    insert( key, root->right );
  }
  else { 
    // ---------------------------------- trace ----------------------------------
    // std::cout << "insert( " << key << " ) found existing Node with ";
    // std::cout << "root->key = " << root->key << std::endl; 
    // ---------------------------------------------------------------------------
    visitor.push_back( height( root ) ); // some compilers do not like empty
    visitor.pop_back();                 // code blocks; these two lines have no effect 
  } 
  /* We know: descendant had a child added (in recursive call, above). */
  /* Now we're "unwinding" the call-stack (returning from the recursive calls, */
  /* one level at a time, until we get to the original call). */
  /* Do we have to rebalance at this level? */
  if( updateHeight(root) ) balance(root);
}

/**
 * Helper to print out the tree sideways. 
 * 
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * @param depth = the number of levels down into the whole tree that *root occurs   
 * @pre root is NULL, or *root is a valid Node
 * @returns nothing 
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
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * @pre root is NULL, or *root is a valid Node 
 * @post the tree rooted at *root is printed in reverse inorder with nodes  
 *       indented 3 * depth spaces; this produces a tree that has its root near  
 *       the left side of the screen and the leaves near the right side.
 * @returns nothing 
 */

void printTree( Node * root ) {
  printTreeHelper( root, 0 );
}

/** 
 * Return a pointer to the Node in the tree with the given key.
 *
 * If a Node with the given key exists in the tree, a pointer to it is returned. 
 * Otherwise NULL is returned.  
 * @param key = the key value to locate
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * @pre root is NULL, or *root is a valid Node
 * @returns a pointer to the Node with the given key, or NULL if not found 
 */ 
Node* findNode(KType key, Node* root) { 
    if (! root) return (Node*) NULL; 
    if (key < root->key) return findNode( key, root->left); 
    if (key > root->key) return findNode( key, root->right); 
    return root; 
}
/**
 * Helper to visit all Nodes in the tree in a reverse inorder traversal. 
 * 
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * @param level = the number of levels down into the whole tree *root occurs   
 * @param visitor = a reference to a std::vector<int> 
 *
 * @pre root is NULL, or *root is a valid Node 
 * @returns nothing 
 */
void visitTreeHelper( Node* root, int level, std::vector<int>& visitor){
  if (! root) return; 
  visitTreeHelper( root->right, level + 1, visitor);
  visitor.push_back( root->key ); 
  visitor.push_back( root->height ); 
  visitor.push_back( level ); 
  visitTreeHelper( root->left, level + 1, visitor);
}  
/** 
 * Visit all Nodes in an AVL tree in a reverse inorder traversal. 
 * 
 * @param root = a Node pointer that contains the address of 
 *        the root of the tree (or subtree)
 * @param visitor = a reference to a std::vector<int> 
 * 
 * @pre root is NULL, or *root is a valid Node 
 * @pre visitor is a vector<int> 
 * @post the actual values of the key, height and level variables (which 
 *       represent the printout of the AVL tree) are in the the visitor vector 
 * @returns nothing 
 */ 
void visitTree(Node* root, std::vector<int>& visitor){ 
  visitTreeHelper( root, 0, visitor);
}
/**
 * Expected values when Unit testing tree traversals with default keys.
 */ 
int traversalValues [] = { 70,0,0,
  70,1,0,10,0,1,
  70,0,1,60,1,0,10,0,1,
  70,0,1,60,2,0,20,0,2,10,1,1,
  70,0,1,60,2,0,50,0,2,20,1,1,10,0,2,
  70,0,2,60,1,1,50,2,0,30,0,2,20,1,1,10,0,2,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,20,2,1,10,0,2,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,25,0,3,20,2,1,10,0,2,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,27,0,3,25,2,1,20,1,2,10,0,3,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,27,0,3,25,2,1,20,0,3,10,1,2,5,0,3 };
  
/** 
 * Load the expected vector with the values expected to be found. 
 * 
 * @param num_to_skip = number of traversalValues elements to skip 
 * @param num_to_take = number of traversalValues elements to load into vector 
 * @param expected = a reference to a std::vector<int> 
 * @pre expected is a vector<int> 
 * @post the expected values of the key, height and level variables (which 
 *       represent the printout of the AVL tree) are in the expected vector 
 * @returns nothing 
 */ 
void prepareExpected(int num_to_skip, int num_to_take, std::vector<int>& expected){
  expected.clear();
  for (int ii = num_to_skip; ii < num_to_skip + num_to_take; ++ ii ){ 
    expected.push_back( traversalValues [ii] ); 
  }
  return;
}
  
/** 
 * the keys to insert into the AVL tree by default
 */ 
int defaultKeys[] = {70, 10, 60, 20, 50, 30, 40, 25, 27, 5};

/**
 * Execute the program with no arguments to use the defaultKeys and run Unit tests, 
 * or supply the keys you want to insert into the AVL tree. 
 */ 
int main( int argc, char *argv[] ) {
  Node *avlTree_root = NULL;
  int *keys;
  int num_keys;

  if( argc == 1 ) {	/* if no command line args use default keys */
    num_keys = sizeof(defaultKeys)/sizeof(int);
    keys = new int[num_keys];
    for( int i=0; i < num_keys; ++i ) {
      keys[i] = defaultKeys[i];
    }
    /* use Unit testing (force unit.isActive() by writing a success)  */
    unit.assertNull("Empty AVL tree has root=NULL", avlTree_root); 
  }
  else {		/* use keys given on command line  */
    num_keys = argc - 1;
    keys = new int[num_keys];
    for( int i=0; i < num_keys; ++i ) {
      keys[i] = atoi(argv[i+1]);
    }
  }

  int unitOffset = 0;
  for( int i=0; i < num_keys; ++i ) {
    std::cout << "Insert: " << keys[i] << std::endl;
    insert(keys[i], avlTree_root);
    std::cout << "Tree:" << std::endl;
    printTree(avlTree_root);
    /* use default keys for unit tests */
    if ( unit.isActive() ) { 
      /* insert(Ktype, Node*&) is suppplied, so root cannot be NULL */ 
      unit.insistNonNull("avlTree_root", avlTree_root); 
      /* verify contains(keys[i],Node*&) is true, because if not, there's */
      /* more than just the shape of the tree that's wrong */
      std::stringstream cmt;
      cmt <<"iteration=" << i << ", contains( " << keys[i];
      cmt << ", " << avlTree_root->key << ") "; 
      unit.assertTrue(cmt.str(), contains( keys[i], avlTree_root )); 
      /* do a reverse inorder traversal, then check whether the visitor */
      /* found the expected values or not */ 
      visitor.clear();
      visitTree(avlTree_root, visitor);
      prepareExpected( 3 * unitOffset, 3 * (i + 1), expected ); 
      unitOffset += i + 1; 
      std::stringstream fmt;
      fmt << "iteration=" << i << ", tree after insert( " << keys[i] << ") "; 
      bool vectorsEqual = unit.assertvectorEquals(fmt.str(), expected, visitor ); 
      if (! vectorsEqual) { 
        /* something is not equal */
        bool hasEqualSize = true; 
        bool hasEqualKeys = true; 
        bool hasEqualHeights = true; 
        bool hasEqualLevels = true; 
        bool hasEqualOrder = true;
        const int vSize = expected.size(); 
        assert( vSize % 3 == 0 ); 
        hasEqualSize = unit.assertEquals("Number of nodes in tree", i + 1, vSize / 3 ); 
        int prevExpected =  expected[0] + 1; 
        int prevVisitor  =  visitor[0] + 1; 
        for (int ii=0; ii < vSize; ii += 3) {
          // expected values MUST be in order, or there's a problem with the test data     
          unit.insist_LT("Expected: curr < prev", expected[ii], prevExpected); 
          prevExpected = expected[ii]; 
          // visitor values SHOULD be in order (or the BST is malformed)     
          hasEqualOrder = hasEqualOrder && unit.assert_LT("Visitor: curr < prev", visitor[ii], prevVisitor); 
          prevVisitor = visitor[ii]; 
          // these should always be equal, since insert() is supplied code 
          if (expected [ii] == visitor[ii]) { 
            std::stringstream hmt;
            hmt << "iteration="<< i << " key=" << expected[ii] << ", height ";
            hasEqualHeights = hasEqualHeights && unit.assertEquals( hmt.str(), expected[ii+1], visitor[ii+1] );
            std::stringstream lmt;
            lmt << "iteration="<< i << " key=" << expected[ii] << ", level ";
            hasEqualLevels  = hasEqualLevels  && unit.assertEquals( lmt.str(), expected[ii+2], visitor[ii+2] ); 
          } 
          else {
            hasEqualKeys = false; 
          }
        }
        if (! hasEqualOrder) { 
          std::cout << "Nodes are not in BST order, processing cannot proceed with a corrupted AVL tree" << std::endl; 
          std::cout << "If you haven't changed any functions except balance() then either you are rotating" << std::endl; 
          std::cout << "nodes that are not unbalanced, or you are using an inappropriate rotation" << std::endl; 
          std::cout << "Uncomment the trace statements to see what functions are being executed." << std::endl; 
          for (int ii=0; ii < vSize; ii += 3) { 
            std::cout << visitor[ii] << " "; 
          }
          std::cout << std::endl; 
          unit.insistTrue("keys in AVL tree must be in BST Order", hasEqualOrder); 
        }
        if (hasEqualSize && hasEqualKeys) { 
          if (! hasEqualHeights)  std::cout << "Iteration=" << i << " node->height values are not correct" << std::endl; 
          if (! hasEqualLevels) {  
            std::cout <<"Iteration=" << i << " expected tree to look like:" << std::endl; 
            for (int ii=0; ii < vSize; ii += 3) {
              std::cout << std::setw( 3 * expected[ii+2] ) << "";	
              std::cout << expected[ii] << std::endl; 
            }
            std::cout << "Are the appropriate rotations being done at the required times?" << std:: endl;  
          }              
        }
        else if (! hasEqualSize) { 
          std::cout << "Expected to find " << expected.size() << "values (which is 3 per Node)" << std::endl; 
          std::cout << "but visitor found " << visitor.size() << "values instead" << std::endl; 
          if (expected.size() < visitor.size() ) { 
            std::cout <<  "More nodes than expected are in the tree, are there duplicates?" << std::endl; 
          }
          else { 
            std::cout << "Less nodes than expected are in the tree, is the defaultKeys[] array okay?" << std::endl; 
          } 
          std::cout << "Uncomment the trace statements to see what functions are being executed." << std::endl; 
        }
        else {
          std::cout << "The test data seems to be out of sync with the default key values." << std::endl; 
          assert(! hasEqualKeys); 
          std::cout << "Iteration=" << i << " key values (and perhaps more) are corrupted" << std::endl; 
        }
      }  
    }         
  }   
  if (unit.isActive() ) unit.printResults(); 

  /* Free up allocated memory  */
  delete[] keys;
  deleteTree(avlTree_root);

  return 0;
}

