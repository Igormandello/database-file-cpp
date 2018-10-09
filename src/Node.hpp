#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include <iostream>

struct Node {
  int data;
  int left;
  int right;

  Node(int i) {
    this->data = i;
    this->left = -1;
    this->right = -1;
  }

  Node() {
    this->data = -1;
    this->left = -1;
    this->right = -1;
  }

  friend std::ostream& operator<<(std::ostream& os, const Node& n) {
    os << "{ data: " << n.data << ", left: " << n.left << ", right: " << n.right << " }";
    return os;
  }
};

#endif