#include <iostream>
#include <string>

using namespace std;

template<typename K, typename V>
class HashNode {
  public:
    HashNode(const K key, const V value) {
      this->key = key;
      this->value = value;
      this->next = nullptr;
    }

    K getKey() {
      return key;
    }

    V getValue() {
      return value;
    }

    void setValue(const V value) {
      this->value = value;
    }

    HashNode * getNext() {
      return next;
    }

    void setNext(HashNode<K, V> * node) {
      this->next = node;
    }

  private:
    K key;
    V value;
    HashNode * next;
};

template<typename K, typename V, typename H = hash<K> >
class HashMap {
  public:
    HashMap(size_t bucketCount = 4096) {
      if (bucketCount == 0) {
        bucketCount = 1;
      }
      this->itemCount = 0;
      this->bucketCount = bucketCount;
      this->buckets = new HashNode<K, V> * [bucketCount]();
    }

    V get(const K key) {
      size_t index = getBucketIndex(key, bucketCount);
      HashNode<K, V> * node = buckets[index];

      while (node != nullptr) {
        if (node->getKey() == key) {
          return node->getValue();
        } else {
          node = node->getNext();
        }
      }
      
      V empty;
      return empty;
    }

    bool set(const K key, const V value) {
      inflateBuckets();
      size_t index = getBucketIndex(key, bucketCount);
      return insert(buckets, index, key, value);
    }

    bool put(const K key, const V value) {
      inflateBuckets();
      size_t index = getBucketIndex(key, bucketCount);
      return insert(buckets, index, key, value, true);
    }

    bool remove(const K key) {
      size_t index = getBucketIndex(key, bucketCount);
      HashNode<K, V> * prevNode = nullptr;
      HashNode<K, V> * node = buckets[index];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node && node->getKey() == key) {
        if (prevNode != nullptr) {
          prevNode->setNext(node->getNext());
        } else {
          buckets[index] = nullptr;
        }
        delete node;
        itemCount--;

        return true;
      }

      return false;
    }

    void clear() {
      clearBuckets(buckets, bucketCount);
    }

    size_t count() {
      return itemCount;
    }

  private:
    size_t itemCount;
    size_t bucketCount;
    HashNode<K, V> ** buckets;
    H hash;

    size_t getBucketIndex(const K key, const size_t bucketCount) {
      return hash(key) % bucketCount;
    }

    bool insert(HashNode<K, V> ** buckets, const size_t index, const K key, const V value, const bool upsert = false) {
      HashNode<K, V> * prevNode = nullptr;
      HashNode<K, V> * node = buckets[index];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node == nullptr) {
        node = new HashNode<K, V>(key, value);
        if (prevNode == nullptr) {
          buckets[index] = node;
        } else {
          prevNode->setNext(node);
        }
        itemCount++; 
        return true;
      }
      
      if (upsert == true) {
        node->setValue(value);
        return true;
      }

      return false;
    }

    void clearBucket(HashNode<K, V> ** buckets, const size_t index) {
      HashNode<K, V> * node = buckets[index];
      while (node != nullptr) {
        HashNode<K, V> * prevNode = node;
        delete prevNode;
        itemCount--;
        node = node->getNext();
      }
      buckets[index] = nullptr;
    }

    void clearBuckets(HashNode<K, V> ** buckets, const size_t bucketCount) {
      for (int index = 0; index < bucketCount; index++) {
        clearBucket(buckets, index);
      }
    }

    void inflateBuckets() {
      const size_t newBucketCount = bucketCount * 2;
      HashNode<K, V> ** newBuckets = new HashNode<K, V> * [newBucketCount]();

      cout << "inflating bucket array from " << bucketCount << endl;

      for (int index = 0; index < bucketCount; index++) {
        HashNode<K, V> * node = buckets[index];
        while (node != nullptr) {
          size_t inflatedIndex = getBucketIndex(node->getKey(), newBucketCount);
          insert(newBuckets, inflatedIndex, node->getKey(), node->getValue());
          node = node->getNext();
        }
        clearBucket(buckets, index);
      }

      delete[] buckets;
      buckets = newBuckets;
      bucketCount = newBucketCount;
    }

    bool bucketsHaveReachedThreshold() {
      return true; // TODO insert threshold check here
    }
};

int main() {
  HashMap<unsigned long int, string> map;

  for (int i = 0; i < 10; i++) {
    map.set(i, "foo" + to_string(i));
    cout << "creating " << map.get(i) << endl;
    cout << "map size = " << map.count() << endl;
  }

  cout << "4 equals " << map.get(4) << endl;
  cout << "5 equals " << map.get(5) << endl;
  cout << "6 equals " << map.get(6) << endl;
  cout << "there are " << map.count() << " items in the map" << endl;
  cout << "deleting key 5" << endl;
  map.remove(5);
  cout << "there are " << map.count() << " items in the map" << endl;
  cout << "4 equals " << map.get(4) << endl;
  cout << "5 equals " << map.get(5) << endl;
  cout << "6 equals " << map.get(6) << endl;
  map.clear();
  cout << "there are " << map.count() << " items in the map" << endl;
  cout << "4 equals " << map.get(4) << endl;

  return 0;
}
