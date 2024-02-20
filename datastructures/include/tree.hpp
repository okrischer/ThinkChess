#include <stdexcept>

namespace tree {

// Map is an ordered container for storing key-value pairs of unique keys
// (no duplicates) based on a binary search tree.
template<typename K, typename V>
class Map {
public:
  Map() : root{nullptr} {}
  ~Map() {}

  // gets the number of entries
  int size() {return size(root);}

  // checks wether the map is empty
  bool isEmpty() {return size() == 0;}

  // returns the value for a given key;
  // if the key is not found, it throws an invalid_argument exception
  V get(K key) {return get(root, key);}

  // checks wether the map contains a given key
  bool contains(K key) {
    try {
      get(key); return true;
    } catch(std::invalid_argument& e) {return false;}
  }

  // inserts a new key-value pair, overwriting the old value
  // if the key already exists
  void put(K key, V value) {root = put(root, key, value);}

protected:
  // Node stores one key-value pair and two pointers
  // to the left and right subtree rooted at this node
  struct Node {
    K key;
    V value;
    int sz;
    Node* left;
    Node* right;

    Node(K k, V v, int s) :
      key{k}, value{v}, sz{s}, left{nullptr}, right{nullptr} {}
    ~Node() {}
  };

private:
  Node* root;

  int size(Node* node) {
    if (!node) return 0;
    return node->sz;
  }

  V get(Node* node, K key) {
    if (node) {
      if (key < node->key) return get(node->left, key);
      if (key > node->key) return get(node->right, key);
      return node->value;
    }
    else throw std::invalid_argument{"Map::get(): key not found"};
  }

  Node* put(Node* node, K key, V value) {
    if (!node) return new Node(key, value, 1);
    if (key < node->key) node->left = put(node->left, key, value);
    else if (key > node->key) node->right = put(node->right, key, value);
    else node->value = value;
    node->sz = 1 + size(node->left) + size(node->right);
    return node;
  }
};

} // end of namespace
