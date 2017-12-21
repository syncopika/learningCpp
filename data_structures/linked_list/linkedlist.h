// linkedlist.h

// create a Node class
// allow for generics!

template<class T>
class Node {

// private instance variables
// value to hold a node's data, and next to point to the next node 
private:
  T value; // allow for generics! - should it be a reference? what if the value is an object? 
  Node *next; // a pointer to the next node 
  
public:
  // constructor
  Node(T val);
  
  // destructor
  ~Node();
  
  // setters
  void setValue(T val);
  void setNext(Node* n); // accept a reference to another node 

  // getters
  T getValue();
  Node* getNext();
  
};