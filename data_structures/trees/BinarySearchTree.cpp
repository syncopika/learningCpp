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
    // do we need to do anything here?
    std::cout << "destroy bst node: " << this->value << '\n';
}


// BinarySearchTree
template <class T>
BinarySearchTree<T>::BinarySearchTree(){
    root = nullptr;
    size = 0;
}

template <class T>
std::unique_ptr<BstNode<T>> BinarySearchTree<T>::addHelper(const T& val, std::unique_ptr<BstNode<T>>& node){
    if(node == nullptr){
        return std::make_unique<BstNode<T>>(val);
    }
    if(val <= node->value){
        node->left = std::move(addHelper(val, node->left));
    }else{
        node->right = std::move(addHelper(val, node->right));
    }
    return std::move(node); // have to move because we're using a unique ptr (cannot be copied)
}

template <class T>
void BinarySearchTree<T>::add(const T& val){
    root = std::move(addHelper(val, root));
    size++;
}

template <class T>
bool BinarySearchTree<T>::removeHelper(const T& val, std::unique_ptr<BstNode<T>>& node){
    // TODO
    // cases:
    // if node to delete has one subtree
    // else if node to delete has 2 subtrees
    // else if node to delete is a leaf
    return true;
}

template <class T>
bool BinarySearchTree<T>::remove(const T& val){
    // TODO
    if(root->value == val && size == 1){
        root = nullptr;
        size--;
    }
    return true;
}

template <class T>
bool BinarySearchTree<T>::findHelper(const T& valToFind, std::unique_ptr<BstNode<T>>& node, bool result){
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
void BinarySearchTree<T>::traverseHelper(std::unique_ptr<BstNode<T>>& node){
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
    // nothing to do since we're using smart pointers :D
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
    std::cout << "size of tree: " << bst.size << "\n";
    
    return 0;
}
