// binary search tree
#ifndef BST_NODE_H
#define BST_NODE_H

#include "Tree.h"

#include <iostream>
#include <memory>
#include <utility>

template <class T>
struct BstNode
{
  T value;
  std::unique_ptr<BstNode<T>> left;
  std::unique_ptr<BstNode<T>> right;
  
  BstNode(const T& value);
        
  ~BstNode();
};

template <class T>
class BinarySearchTree : public Tree<T>
{
  private:
    // helper methods
    std::unique_ptr<BstNode<T>> addHelper(const T& val, std::unique_ptr<BstNode<T>>& node);
    bool removeHelper(const T& val, std::unique_ptr<BstNode<T>>& node);
    bool findHelper(const T& valToFind, std::unique_ptr<BstNode<T>>& node, bool result);
    void traverseHelper(std::unique_ptr<BstNode<T>>& node);

  public:
    int size;
    std::unique_ptr<BstNode<T>> root;
    
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