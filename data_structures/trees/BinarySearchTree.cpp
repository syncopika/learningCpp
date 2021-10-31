// binary search tree

#include "BinarySearchTree.h"
#include <iostream>

// BstNode
template <class T>
BstNode<T>::BstNode(const T& val){
    value = val;
    left = nullptr;
    right = nullptr;
}

template <class T>
BstNode<T>::~BstNode<T>(){
    // not sure what to do here
}


// BinarySearchTree
template <class T>
BinarySearchTree<T>::BinarySearchTree(){
    root = nullptr;
}

template <class T>
BstNode<T>* BinarySearchTree<T>::addHelper(const T& val, BstNode<T>* node){
    if(node == nullptr){
        return new BstNode<T>(val);
    }
    if(val <= node->value){
        node->left = addHelper(val, node->left);
    }else{
        node->right = addHelper(val, node->right);
    }
    return node;
}

template <class T>
void BinarySearchTree<T>::add(const T& val){
    root = addHelper(val, root);
}

template <class T>
bool BinarySearchTree<T>::removeHelper(const T& val, BstNode<T>* node){
    // TODO
    return true;
}

template <class T>
bool BinarySearchTree<T>::remove(const T& val){
    // TODO
    return true;
}

template <class T>
bool BinarySearchTree<T>::findHelper(const T& valToFind, BstNode<T>* node, bool result){
    if(node == nullptr){
        return result;
    }else if(node->value == valToFind){
        return true;
    }else{
        if(valToFind < node->value){
            return findHelper(valToFind, node->left, result || false);
        }else{
            return findHelper(valToFind, node->right, result || false);
        }
    }
}

template <class T>
bool BinarySearchTree<T>::find(const T& valToFind){
    return findHelper(valToFind, root, false);
}

template <class T>
void BinarySearchTree<T>::traverseHelper(BstNode<T>* node){
    if(node == nullptr){
        std::cout << "null\n";
        return;
    }
    traverseHelper(node->left);
    std::cout << "value: " << node->value << '\n';
    traverseHelper(node->right);
}

template <class T>
void BinarySearchTree<T>::traverse(){
    // inorder traversal
    traverseHelper(root);
}

template <class T>
BinarySearchTree<T>::~BinarySearchTree<T>(){
    // deallocate all BstNodes
}


// test
int main(void){
    
    BinarySearchTree<int> bst;
    bst.add(50);
    bst.add(100);
    bst.add(20);
    
    bst.traverse();
    //std::cout << "value: " << tree.getValue() << "\n";
    
    std::cout << "------\n";
    
    std::cout << "50 in bst: " << bst.find(50) << "\n";
    std::cout << "100 in bst: " << bst.find(100) << "\n";
    std::cout << "20 in bst: " << bst.find(20) << "\n";
    std::cout << "51 in bst: " << bst.find(51) << "\n";
    
    return 0;
}
