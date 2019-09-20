#pragma once
#ifndef _CALCSTACK_H
#define _CALCSTACK_H
#include <cassert>
#include <iostream>

template <typename T>
class stack {
  struct node {
    T elem;
    node* next{nullptr};
    explicit node(const T& elem) : elem(elem){};
    node() = default;
  } * head;

  size_t sz;

 public:
  stack() {
    head = new node();
    sz = 0;
  }

  void push(const T& t) {
    node* nw = new node(t);
    nw->next = head->next;
    head->next = nw;

    ++sz;
  }

  bool empty() { return head->next == nullptr; }

  T top() {
    assert(!empty());
    return head->next->elem;
  }

  size_t size() { return sz; }

  void pop() {
    if (!empty()) {
      node* nw = head->next->next;
      delete head->next;
      head->next = nw;

      --sz;
    }
  }

  void clear() {
    while (!empty())
      pop();
  }

  ~stack() { clear(); }
};

#endif  // stack.h