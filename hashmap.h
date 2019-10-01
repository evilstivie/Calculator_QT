#pragma once
#ifndef _CALCHASHMAP_H
#define _CALCHASHMAP_H
#include <iostream>
#include <string>

const int TABLE_SIZE = 1e7 + 33;

template <typename K, typename V>
class hashnode {
 public:
  hashnode(const K& key, const V& value)
      : key(key), value(value), next(nullptr) {}
  K getKey() const { return key; }
  V getValue() const { return value; }
  void setValue(V value) { hashnode::value = value; }
  hashnode* getNext() const { return next; }
  void setNext(hashnode* next) { hashnode::next = next; }

 private:
  K key;
  V value;
  hashnode* next;
};

template <typename K>
struct keyhash {
  unsigned long operator()(const K& key) const {
    return reinterpret_cast<unsigned long>(key) % TABLE_SIZE;
  }
};

template <typename K, typename V, typename F = keyhash<K>>
class hashmap {
 public:
  hashmap() { table = new hashnode<K, V>*[TABLE_SIZE](); }

  ~hashmap() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
      hashnode<K, V>* entry = table[i];
      while (entry != nullptr) {
        hashnode<K, V>* prev = entry;
        entry = entry->getNext();
        delete prev;
      }
      table[i] = nullptr;
    }
    delete[] table;
  }

  bool get(const K& key, V& value) {
    unsigned long hashValue = hashFunc(key);
    hashnode<K, V>* entry = table[hashValue];

    while (entry != nullptr) {
      if (entry->getKey() == key) {
        value = entry->getValue();
        return true;
      }
      entry = entry->getNext();
    }
    return false;
  }

  bool contains(const K& key) {
    unsigned long hashValue = hashFunc(key);
    hashnode<K, V>* entry = table[hashValue];

    while (entry != nullptr) {
      if (entry->getKey() == key) {
        return true;
      }
      entry = entry->getNext();
    }

    return false;
  }

  V get(const K& key) {
    unsigned long hashValue = hashFunc(key);

    hashnode<K, V>* entry = table[hashValue];

    while (entry != nullptr) {
      if (entry->getKey() == key) {
        return entry->getValue();
      }
      entry = entry->getNext();
    }

    return 0;
  }

  void put(const K& key, const V& value) {
    unsigned long hashValue = hashFunc(key);
    hashnode<K, V>* prev = nullptr;
    hashnode<K, V>* entry = table[hashValue];

    while (entry != nullptr && entry->getKey() != key) {
      prev = entry;
      entry = entry->getNext();
    }

    if (entry == nullptr) {
      entry = new hashnode<K, V>(key, value);
      if (prev == nullptr) {
        table[hashValue] = entry;
      } else {
        prev->setNext(entry);
      }
    } else {
      entry->setValue(value);
    }
  }

  void remove(const K& key) {
    unsigned long hashValue = hashFunc(key);
    hashnode<K, V>* prev = nullptr;
    hashnode<K, V>* entry = table[hashValue];

    while (entry != nullptr && entry->getKey() != key) {
      prev = entry;
      entry = entry->getNext();
    }

    if (entry == nullptr) {
      return;
    } else {
      if (prev == nullptr) {
        table[hashValue] = entry->getNext();
      } else {
        prev->setNext(entry->getNext());
      }
      delete entry;
    }
  }

 private:
  hashnode<K, V>** table;
  F hashFunc;
};

struct string_hash {
  const int p = 31;
  unsigned long operator()(const std::string& str) {
    unsigned long hash = 0;
    unsigned long power = 1;
    for (char c : str) {
      hash = (hash + (c - 'a' + 1) * power) % TABLE_SIZE;
      power = (power * p) % TABLE_SIZE;
    }
    return hash;
  }
};

#endif  // hashmap.h
