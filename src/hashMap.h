#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <iostream>
#include <memory>
#include "hashMapNode.h"

namespace kvmap {
  template<typename K, typename V, typename H = std::hash<K> >
  class HashMap {
    public:
      HashMap(std::size_t bucketCount = 16) {
        _nodeCount.reset();
        _bucketCount = bucketCount;
        _bucketArr = new HashMapNode<K, V>* [bucketCount]();
        _endNode = new HashMapNode<K, V>(1, 2);
      }

      ~HashMap() {
        clearBucketArr(_bucketArr, _bucketCount);
        delete[] _bucketArr;
        _bucketArr = nullptr;
      }

      bool exists(const K key) {
        std::size_t index = getBucketIndex(key, _bucketCount);
        HashMapNode<K, V>* node = _bucketArr[index];

        while (node != nullptr) {
          if (node->getKey() == key) {
            return true;
          } else {
            node = node->getNext();
          }
        }

        return false;
      }

      V get(const K key) {
        std::size_t index = getBucketIndex(key, _bucketCount);
        HashMapNode<K, V>* node = _bucketArr[index];

        while (node != nullptr) {
          if (node->getKey() == key) {
            return node->getValue();
          } else {
            node = node->getNext();
          }
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
        HashMapNode<K, V>* prevNode = nullptr;
        HashMapNode<K, V>* node = _bucketArr[index];

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
      HashMapNode<K, V>** _bucketArr;
      HashMapNode<K, V>* _endNode;
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

      bool addToBucket(HashMapNode<K, V>** bucketArr, const std::size_t index, const K key, const V value, const bool upsert = false) {
        HashMapNode<K, V>* prevNode = nullptr;
        HashMapNode<K, V>* node = bucketArr[index];

        while (node != nullptr && node->getKey() != key) {
          prevNode = node;
          node = node->getNext();
        }

        if (node == nullptr) {
          //std::shared_ptr<HashMapNode<K, V>> node = std::make_shared<HashMapNode<K, V>>(key, value);
          node = new HashMapNode<K, V>(key, value);
          if (prevNode == nullptr) {
            bucketArr[index] = node;
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

      void clearBucket(HashMapNode<K, V>** bucketArr, const std::size_t index) {
        HashMapNode<K, V>* node = bucketArr[index];
        while (node != nullptr) {
          HashMapNode<K, V>* prevNode = node;
          node = node->getNext();
          delete prevNode;
        }
      }

      void clearBucketArr(HashMapNode<K, V>** bucketArr, const std::size_t bucketCount) {
        for (int index = 0; index < bucketCount; index++) {
          clearBucket(bucketArr, index);
        }
      }

      void inflateBucketArr() {
        const std::size_t newBucketCount = _bucketCount * 2;
        HashMapNode<K, V>** newBucketArr = new HashMapNode<K, V>* [newBucketCount]();

        for (int bucketArrIndex = 0; bucketArrIndex < _bucketCount; bucketArrIndex++) {
          HashMapNode<K, V>* node = _bucketArr[bucketArrIndex];
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
