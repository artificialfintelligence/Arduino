#ifndef INVDBLLL_H
#define INVDBLLL_H

#include <stdio.h>

template <typename T>
struct Node {
  T data;
  Node<T>* prev;
  Node<T>* next;
};

template <typename T, size_t max_len>
class InvertedDoubleLinkedList {
private:
  Node<T>* head_;
  Node<T>* tail_;
  size_t size_;

public:
  InvertedDoubleLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

  bool HeadAppend(T data) {
    if (size_ >= max_len) {
      // List is at max capacity, cannot add more nodes
      return false;
    }

    Node<T>* new_node = new Node<T>;
    new_node->data = data;
    new_node->next = nullptr;
    new_node->prev = head_;
    if (head_ != nullptr) {
      head_->next = new_node;
    }
    else {
      tail_ = new_node;
    }
    head_ = new_node;
    size_++;
    return true;
  }

  bool TailDelete() {
    if (size_ == 0) {
      // List is empty, cannot delete any nodes
      return false;
    }

    Node<T>* temp = tail_;
    tail_ = temp->next;
    if  (size_ > 1) {
      tail_->prev = nullptr;
    }
    else {
      head_ = nullptr;
    }
    delete temp;
    size_--;
    return true;
  }

  Node<T>* GetHead() const {
    return head_;
  }

  Node<T>* GetTail() const {
    return tail_;
  }
};

#endif // INVDBLLL_H
