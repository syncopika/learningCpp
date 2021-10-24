// binary search tree
#ifndef BST_NODE_H
#define BST_NODE_H

#include "TreeNode.h"

template <class T>
class BSTNode : public TreeNode<T>
{
public:
  BSTNode(const T& value);
	
  ~BSTNode();
	
  // methods
  const T& getValue();
  void add(const T& val);
  bool remove(const T& val);
  bool find(const T& val); 
};

#endif