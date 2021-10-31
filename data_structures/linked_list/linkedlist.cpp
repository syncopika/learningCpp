#include <iostream>
#include "linkedlist.h"

// implement constructor
template <class T>
Node<T>::Node(const T& val){
    value = val;
    next = nullptr;
}

// implement destructor
template <class T>
Node<T>::~Node(){
    // nothing is dynamically allocated?
}

// implement setters
template <class T>
void Node<T>::setValue(const T& val){
    value = val;
}

template <class T>
void Node<T>::setNext(Node* n){
    next = n;
}

// getters
template <class T>
T& Node<T>::getValue() const{
    return value;
}

// if next is null, return null. otherwise, return the ptr to the next node
template <class T>
Node<T>* Node<T>::getNext() const{
    if(next == nullptr){
        return nullptr;
    }else{
        return next;
    }
}


//// testing 
int main(){
    // set up nodes for integer 
    Node<int> n1{10};
    Node<int> n2{3};
    Node<int> n3{5};
    
    // link nodes 
    n1.setNext(&n2);
    n2.setNext(&n3);
    
    Node<int>* curr = &n1;
    
    while(curr != nullptr){
        std::cout << (*curr).getValue() << std::endl;
        curr = (*curr).getNext(); 
    }
    
    std::cout << "------------" << std::endl;
    
    // try it with strings
    Node<std::string> n4("hey");
    
    std::string s2("there");
    Node<std::string> n5(s2);
    
    std::string s3("hiii");
    Node<std::string> n6(s3);
    
    n4.setNext(&n5);
    n5.setNext(&n6);
    
    Node<std::string>* curr2 = &n4;
    
    while(curr2 != nullptr){
        std::cout << (*curr2).getValue() << std::endl;
        curr2 = (*curr2).getNext(); 
    }
    
    return 0;
}
    
    