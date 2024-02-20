#pragma once
#include <stdexcept>

namespace list {

// Public interface for lists, implemented by Queue.
template<typename T>
class LL {
public:
  virtual ~LL() {}
  virtual int size() const = 0;
  virtual T peek(int n) const = 0;
  virtual T pop() = 0;
  virtual void push_back(T elem) = 0;
  virtual void push_front(T elem) = 0;
};

// List implements both FIFO and LIFO data structures based on linked lists.
// FIFO elements are added to the tail, using push_back().
// LIFO elements are added to the head, using push_front().
// Make sure to use only *one* of these methods for every instance of Queue.
// In both cases, the next element is taken from the head, using pop().
template<typename T>
class List : public LL<T> {
public:
  List() : head{nullptr}, tail{nullptr}, sz{0} {}
  ~List() {
    Node* node = head;
    while(node) {
      Node* curr = node;
      node = node->next;
      delete curr;
    }
  }

  // size returns the current size of the list.
  int size() const override {return sz;}

  // peek returns the element with the given index (starting with 1).
  // If there is no element at the given index, an exception is thrown.
  T peek(int n) const override {
    Node* node = head;
    for (int i = 1; i < n && i < sz; i++) {
      node = node->next;
    }
    if (node) return node->data;
    else throw std::out_of_range{"List::peek(): empty list"};
  }

  // pop removes and returns the head of the list.
  // If the list is empty, an exception is thrown.
  T pop() override {
    if (head) {
      Node* first = head;
      T elem = head->data;
      head = head->next;
      delete first;
      sz--;
      return elem;
    } else throw std::out_of_range{"List::pop(): empty list"};
  }

  // push_front inserts a new element at the head of the list.
  void push_front(T elem) override {
    head = new Node(elem, head);
    if (!tail) tail = head;
    sz++;
  }
  // push_back inserts a new element at the tail of the list.
  void push_back(T elem) override {
    if (!tail) {
      head = new Node(elem, head);
      tail = head;
    } else {
      tail->next = new Node(elem, nullptr);
      tail = tail->next;
    }
    sz++;
  }

protected:
  // Node stores one element of the given type
  // and a pointer to the next element.
  struct Node {
    T data;
    Node* next;

    Node(T d, Node* n) : data{d}, next{n} {}
    ~Node() {}
  };

private:
  Node* head;
  Node* tail;
  int sz;
};
} // end of namespace list
