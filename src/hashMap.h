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
      HashMap(std::size_t count = 16) : _bucketCount(count) {
        _nodeCount.reset();
        _bucketArr = new Node* [_bucketCount]();
        _endNode = new Node(1, 2);
      }

      ~HashMap() {
        clearBucketArr(_bucketArr, _bucketCount);
        delete[] _bucketArr;
        _bucketArr = nullptr;
      }

      bool exists(const K key) {
        std::size_t index = getBucketIndex(key, _bucketCount);
        Node* node = _bucketArr[index];

        while (node != nullptr) {
          if (node->getKey() == key) {
            return true;
          }
          
          node = node->getNext();
        }

        return false;
      }

      V get(const K key) {
        std::size_t index = getBucketIndex(key, _bucketCount);
        Node* node = _bucketArr[index];

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
        std::size_t index = getBucketIndex(key, _bucketCount);
        Node* prevNode = nullptr;
        Node* node = _bucketArr[index];

        while (node != nullptr && node->getKey() != key) {
          prevNode = node;
          node = node->getNext();
        }

        if (node && node->getKey() == key) {
          if (prevNode != nullptr) {
            prevNode->setNext(node->getNext());
          } else {
            _bucketArr[index] = nullptr;
          }

          _nodeCount.decrement();

          delete node;
          return true;
        }

        return false;
      }

      void clear() {
        _nodeCount.reset();
        clearBucketArr(_bucketArr, _bucketCount);
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
      };

      nodeCounter _nodeCount;
      std::size_t _bucketCount;
      Node** _bucketArr;
      Node* _endNode;
      H hashFunc;

      std::size_t getBucketIndex(const K key, const std::size_t bucketCount) const {
        return hashFunc(key) % bucketCount;
      }

      bool insert(const K key, const V value, const bool upsert = false) {
        if (bucketArrHasReachedThreshold()) {
          inflateBucketArr();
        }
        _nodeCount.increment();
        std::size_t index = getBucketIndex(key, _bucketCount);
        return addToBucket(_bucketArr, index, key, value, upsert);
      }

      bool addToBucket(Node** bucketArr, const std::size_t index, const K key, const V value, const bool upsert = false) {
        Node* prevNode = nullptr;
        Node* node = bucketArr[index];

        while (node != nullptr && node->getKey() != key) {
          prevNode = node;
          node = node->getNext();
        }

        if (node == nullptr) {
          node = new Node(key, value);
          std::unique_ptr<Node> node2 = std::make_unique<Node>(key, value);
          if (prevNode == nullptr) {
            bucketArr[index] = node;
          } else {
            prevNode->setNext(node);
            prevNode->setNextNode(std::move(node2));
          }
          return true;
        }
        
        if (upsert == true) {
          node->setValue(value);
          return true;
        }

        return false;
      }

      void clearBucket(Node** bucketArr, const std::size_t index) {
        Node* node = bucketArr[index];
        while (node != nullptr) {
          Node* prevNode = node;
          node = node->getNext();
          delete prevNode;
        }
      }

      void clearBucketArr(Node** bucketArr, const std::size_t bucketCount) {
        for (int index = 0; index < bucketCount; index++) {
          clearBucket(bucketArr, index);
        }
      }

      void inflateBucketArr() {
        const std::size_t newBucketCount = _bucketCount * 2;
        Node** newBucketArr = new Node* [newBucketCount]();

        for (int bucketArrIndex = 0; bucketArrIndex < _bucketCount; bucketArrIndex++) {
          Node* node = _bucketArr[bucketArrIndex];
          while (node != nullptr) {
            std::size_t newBucketArrIndex = getBucketIndex(node->getKey(), newBucketCount);
            addToBucket(newBucketArr, newBucketArrIndex, node->getKey(), node->getValue());
            node = node->getNext();
          }
          clearBucket(_bucketArr, bucketArrIndex);
        }

        delete[] _bucketArr;
        _bucketArr = newBucketArr;
        _bucketCount = newBucketCount;
      }

      const bool bucketArrHasReachedThreshold() const {
        return _nodeCount.val >= _bucketCount * 0.6;
      }
  };
}

#endif /* HASHMAP_H_ */

