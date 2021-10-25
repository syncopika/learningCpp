#ifndef TREE_NODE_H
#define TREE_NODE_H

template<class T>
class TreeNode {
protected:
  T value;
  //TreeNode* left;
  //TreeNode* right;
  
public:
  // constructor
  TreeNode(const T& val) : value(val) {}
  
  virtual ~TreeNode() {}
  
  const T& getValue() const;
  virtual void add(const T& val) = 0;
  virtual bool remove(const T& val) = 0;
  virtual bool find(const T& val) = 0;
  virtual void traverse() = 0;
};

#endif