#ifndef TREE_H
#define TREE_H

// acts like an abstract interface
template<class T>
class Tree {
  public:
    virtual void add(const T& val) = 0;
    virtual bool remove(const T& val) = 0;
    virtual bool find(const T& val) = 0;
    virtual void traverse() = 0;
};

#endif