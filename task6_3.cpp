#include <iostream>
#include <stack>


template <typename T>
struct Node {
  Node<T>* left;
  Node<T>* right;
  T value;

  Node(T val) : value(val), left(nullptr), right(nullptr) {}
};


template <typename T>
void print_node(Node<T>* node) {
  std::cout << node->value << std::endl;
}

template <typename T>
void delete_node(Node<T>* node) {
  delete node;
}


template <typename T>
class BinaryTree {
 private:
  Node<T>* root;

 public:
  BinaryTree() : root(nullptr) {}
  ~BinaryTree();
  Node<T>* find(T val);
  bool insert(T val);
  Node<T>* get_root();
};

template <typename T>
Node<T>* BinaryTree<T>::find(T val) {
  Node<T>* parent = nullptr;
  Node<T>* node = root;
  while (node) {
    parent = node;
    if (node->value == val) {
      return node;
    } else if (node->value < val) {
      node = node->right;
    } else {
      node = node->left;
    }
  }
  return parent;
}

template <typename T>
bool BinaryTree<T>::insert(T val) {
  auto node = find(val);
  if (!node) {
    root = new Node<T>(val);
  } else if (node->value == val) {
    return false;
  } else {
    if (node->value > val) {
      node->left = new Node<T>(val);
    } else {
      node->right = new Node<T>(val);
    }
  }
  return true;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
  postorder(root, delete_node);
}

template <typename T>
Node<T>* BinaryTree<T>::get_root() {
  return root;
}


template <typename T>
void postorder(Node<T>* root, void (*node_process)(Node<T>*)) {
  std::stack<Node<T>*> stack;
  stack.push(nullptr);
  while (root) {
    if (root->left) {
      stack.push(root);
      root = root->left;
    } else if (root->right) {
      stack.push(root);
      root = root->right;
    } else {
      Node<T>* prev = root;
      while (root && (!root->right || root->right == prev)) {
        node_process(root);
        prev = root;
        root = stack.top();
        stack.pop();
      }
      if (root) {
        stack.push(root);
        root = root->right;
      }
    }
  }
}


int main() {
  int n;
  std::cin >> n;
  BinaryTree<int> tree;
  while (n) {
    int val;
    std::cin >> val;
    tree.insert(val);
    --n;
  }
  postorder(tree.get_root(), print_node);
  return 0;
}