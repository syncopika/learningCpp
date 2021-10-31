// linkedlist.h

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template<class T>
class Node {

// private instance variables
// value to hold a node's data, and next to point to the next node 
private:
  T value;
  Node *next; // a pointer to the next node 
  
public:
  // constructor
  // note that since we want references passed in (to prevent excessive copying, especially for big objects/structs), 
  // we have to make it const as well because we might be passing in a temporary object (e.g. some int like 5 or a string)
  Node(const T& val);
  
  // destructor
  ~Node();
  
  // setters
  void setValue(const T& val);
  void setNext(Node* n); // accept a reference to another node 

  // getters
  T& getValue() const;
  Node* getNext() const;
  
};

#endif