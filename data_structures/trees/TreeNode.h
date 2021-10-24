#ifndef TREE_NODE_H
#define TREE_NODE_H

template<class T>
class TreeNode {
protected:
  T value;
  TreeNode* left;
  TreeNode* right;
  
public:
  const T& getValue() const;
  virtual void add(const T& val) = 0;
  virtual bool remove(const T& val) = 0;
  virtual bool find(const T& val) = 0;
};

#endif