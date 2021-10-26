// binary search tree
#ifndef BST_NODE_H
#define BST_NODE_H

#include "TreeNode.h"

template <class T>
class BSTNode : public TreeNode<T>
{
private:
  // helper methods
  BSTNode* addHelper(const T& val, BSTNode* node);
  bool removeHelper(const T& val, BSTNode* node);
  bool findHelper(const T& valToFind, BSTNode* node, bool result);
  void traverseHelper(BSTNode* node);

public:
  // constructor
  BSTNode(const T& value);
  
  // destructor
  ~BSTNode();
  
  // member variables
  BSTNode* left;
  BSTNode* right;
	
  // methods
  const T& getValue() const; // this one might not be necessary?
  void add(const T& val);
  bool remove(const T& val);
  bool find(const T& valToFind); 
  void traverse();
};

#endif