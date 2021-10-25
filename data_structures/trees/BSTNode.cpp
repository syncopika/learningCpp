// binary search tree

#include "BSTNode.h"
#include <iostream>

template <class T>
BSTNode<T>::BSTNode(const T& value): TreeNode<T>(value){
	left = nullptr;
	right = nullptr;
}

template <class T>
const T& BSTNode<T>::getValue() const{
	return this->value;
}

template <class T>
BSTNode<T>* BSTNode<T>::addHelper(const T& val, BSTNode* node){
	if(node == nullptr){
		return new BSTNode(val);
	}
	if(val <= node->getValue()){
		node->left = addHelper(val, node->left);
		return node->left;
	}else{
		node->right = addHelper(val, node->right);
		return node->right;
	}
}

template <class T>
void BSTNode<T>::add(const T& val){
	// we actually should have a BstTree class that uses this node.
	// the tree will have a root pointer, which then can be passed to addHelper.
	// then this method should belong to the tree.
	// or we can do this
	if(left == nullptr && val < this->value){
		left = new BSTNode(val);
	}else if(right == nullptr && val > this->value){
		right = new BSTNode(val);
	}else if(val <= this->value){
		left = addHelper(val, left);
	}else{
		right = addHelper(val, right);
	}
}

template <class T>
bool BSTNode<T>::remove(const T& val){
	// TODO
	return true;
}

template <class T>
bool BSTNode<T>::find(const T& val){
	// TODO
	// recursive helper?
	return true;
}

template <class T>
void BSTNode<T>::traverseHelper(BSTNode* node){
	if(node == nullptr){
		std::cout << "null\n";
		return;
	}
	traverseHelper(node->left);
	std::cout << "value: " << node->value << '\n';
	traverseHelper(node->right);
}

template <class T>
void BSTNode<T>::traverse(){
	// inorder traversal
	traverseHelper(this);
}

template <class T>
BSTNode<T>::~BSTNode(){
	// anything to do? delete left and right?
}


// test
int main(void){

	BSTNode<int> node{50};
	node.add(100);
	
	node.traverse();
	//std::cout << "value: " << node.getValue() << "\n";

	return 0;
}