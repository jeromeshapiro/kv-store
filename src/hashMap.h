#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <iostream>
#include <memory>
#include "hashMapNode.h"

namespace kvmap {
  template<typename K, typename V, typename H = std::hash<K>>
  class HashMap {
    typedef HashMapNode<K, V> Node;

    public:
      HashMap(const std::size_t count = 16) : _numberOfBuckets(count), _initialNumberOfBuckets(count) {
        _buckets = new Node* [_initialNumberOfBuckets]();
      }

      ~HashMap() {
        deleteAllBuckets();
        delete[] _buckets;
        _buckets = nullptr;
      }

      bool exists(const K key) {
        const std::size_t bucketIndex = getBucketIndex(key, _numberOfBuckets);
        Node* node = _buckets[bucketIndex];

        while (node != nullptr) {
          if (node->getKey() == key) {
            return true;
          }
          node = node->getNext();
        }

        return false;
      }

      V get(const K key) {
        const std::size_t bucketIndex = getBucketIndex(key, _numberOfBuckets);
        Node* node = _buckets[bucketIndex];

        while (node != nullptr) {
          if (node->getKey() == key) {
            return node->getValue();
          }
          node = node->getNext();
        }
        
        return 0;
      }

      bool set(const K key, const V value) {
        return insert(key, value);
      }

      bool put(const K key, const V value) {
        return insert(key, value, true);
      }

      bool remove(const K key) {
        const std::size_t index = getBucketIndex(key, _numberOfBuckets);
        Node* prevNode = nullptr;
        Node* node = _buckets[index];

        while (node != nullptr && node->getKey() != key) {
          prevNode = node;
          node = node->getNext();
        }

        if (node && node->getKey() == key) {
          if (prevNode != nullptr) {
            prevNode->setNext(node->getNext());
          } else {
            _buckets[index] = nullptr;
          }

          _nodeCount.decrement();
          delete node;
          node = nullptr;

          return true;
        }

        return false;
      }

      void clear() {
        _nodeCount.reset();
        deleteAllBuckets();
        delete[] _buckets;
        _numberOfBuckets = _initialNumberOfBuckets;
        _buckets = new Node* [_initialNumberOfBuckets]();
      }

      std::size_t count() const {
        return _nodeCount.val;
      }

    private:
      struct nodeCounter {
        std::size_t val = 0;
        void increment() { val++; }
        void decrement() { val--; }
        void reset() { val = 0; }
      } _nodeCount;

      std::size_t _numberOfBuckets, _initialNumberOfBuckets;
      Node** _buckets;
      H hashFunc;

      std::size_t getBucketIndex(const K key, const std::size_t bucketCount) const {
        return hashFunc(key) % bucketCount;
      }

      bool insert(const K key, const V value, const bool upsert = false) {
        if (bucketsHaveReachedThreshold()) {
          inflateBuckets();
        }
        _nodeCount.increment();
        const std::size_t bucketIndex = getBucketIndex(key, _numberOfBuckets);
        return addToBucket(_buckets, bucketIndex, key, value, upsert);
      }

      bool addToBucket(Node** buckets, const std::size_t bucketIndex, const K key, const V value, const bool upsert = false) {
        Node* prevNode = nullptr;
        Node* node = buckets[bucketIndex];

        while (node != nullptr && node->getKey() != key) {
          prevNode = node;
          node = node->getNext();
        }

        if (node == nullptr) {
          node = new Node(key, value);
          if (prevNode == nullptr) {
            buckets[bucketIndex] = node;
          } else {
            prevNode->setNext(node);
          }
          return true;
        }
        
        if (upsert == true) {
          node->setValue(value);
          return true;
        }

        return false;
      }

      void deleteBucket(const std::size_t bucketIndex) {
        Node* prevNode = nullptr;
        Node* node = _buckets[bucketIndex];
        while (node != nullptr) {
          prevNode = node;
          node = node->getNext();
          delete prevNode;
          prevNode = nullptr;
        }
      }

      void deleteAllBuckets() {
        for (int i = 0; i < _numberOfBuckets; i++) {
          deleteBucket(i);
        }
      }

      void inflateBuckets() {
        const std::size_t newNumberOfBuckets = _numberOfBuckets * 2;
        Node** newBuckets = new Node* [newNumberOfBuckets]();

        for (int bucketIndex = 0; bucketIndex < _numberOfBuckets; bucketIndex++) {
          Node* node = _buckets[bucketIndex];
          while (node != nullptr) {
            const std::size_t newBucketIndex = getBucketIndex(node->getKey(), newNumberOfBuckets);
            addToBucket(newBuckets, newBucketIndex, node->getKey(), node->getValue());
            node = node->getNext();
          }
          deleteBucket(bucketIndex);
        }

        delete[] _buckets;
        _buckets = newBuckets;
        _numberOfBuckets = newNumberOfBuckets;
      }

      const bool bucketsHaveReachedThreshold() const {
        return _nodeCount.val >= _numberOfBuckets * 0.6;
      }
  };
}

#endif /* HASHMAP_H_ */

