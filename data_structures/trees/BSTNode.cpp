// binary search tree

#include "BSTNode.h"
#include <iostream>

template <class T>
BSTNode<T>::BSTNode(const T& value){
	this->value = value;
	TreeNode<T>::left = nullptr;
	TreeNode<T>::right = nullptr;
}

template <class T>
const T& BSTNode<T>::getValue(){
	return this->value;
}

template <class T>
void BSTNode<T>::add(const T& val){
	// TODO
}

template <class T>
bool BSTNode<T>::remove(const T& val){
	// TODO
	return true;
}

template <class T>
bool BSTNode<T>::find(const T& val){
	// TODO
	return true;
}

template <class T>
BSTNode<T>::~BSTNode(){
	// nothing to do
}


// test
int main(void){

	BSTNode<int> node{50};
	
	std::cout << "value: " << node.getValue() << "\n";

	return 0;
}