#ifndef NODE_INCLUDED
#define NODE_INCLUDED

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
};

#endif