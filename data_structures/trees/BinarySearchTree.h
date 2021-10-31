// binary search tree
#ifndef BST_NODE_H
#define BST_NODE_H

#include "Tree.h"

template <class T>
struct BstNode
{
  T value;
  BstNode<T>* left;
  BstNode<T>* right;
        
  BstNode(const T& value);
        
  ~BstNode();
};

template <class T>
class BinarySearchTree : public Tree<T>
{
  private:
    // helper methods
    BstNode<T>* addHelper(const T& val, BstNode<T>* node);
    bool removeHelper(const T& val, BstNode<T>* node);
    bool findHelper(const T& valToFind, BstNode<T>* node, bool result);
    void traverseHelper(BstNode<T>* node);

  public:
    BstNode<T>* root;
    
    // constructor
    BinarySearchTree();
  
    // destructor
    ~BinarySearchTree();
    
    // methods
    void add(const T& val) override;
    bool remove(const T& val) override;
    bool find(const T& valToFind) override; 
    void traverse() override;
};

#endif