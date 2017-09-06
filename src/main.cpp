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

    const K getKey() const {
      return key;
    }

    const V getValue() const {
      return value;
    }

    void setValue(const V value) {
      this->value = value;
    }

    HashNode* getNext() const {
      return next;
    }

    void setNext(HashNode<K, V>* node) {
      this->next = node;
    }

  private:
    K key;
    V value;
    HashNode* next;
};

template<typename K, typename V, typename H = hash<K> >
class HashMap {
  public:
    HashMap(size_t bucketCount = 16) {
      if (bucketCount == 0) {
        bucketCount = 1;
      }
      this->itemCount = 0;
      this->bucketCount = bucketCount;
      this->bucketArr = new HashNode<K, V>* [bucketCount]();
    }

    V get(const K key) {
      size_t index = getBucketIndex(key, bucketCount);
      HashNode<K, V>* node = this->bucketArr[index];

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
      return insert(key, value);
    }

    bool put(const K key, const V value) {
      return insert(key, value, true);
    }

    bool remove(const K key) {
      size_t index = getBucketIndex(key, bucketCount);
      HashNode<K, V>* prevNode = nullptr;
      HashNode<K, V>* node = this->bucketArr[index];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node && node->getKey() == key) {
        if (prevNode != nullptr) {
          prevNode->setNext(node->getNext());
        } else {
          this->bucketArr[index] = nullptr;
        }
        this->itemCount--;
        delete node;
        return true;
      }

      return false;
    }

    void clear() {
      this->itemCount = 0;
      clearBucketArr(bucketArr, bucketCount);
    }

    size_t count() const {
      return itemCount;
    }

  private:
    size_t itemCount;
    size_t bucketCount;
    HashNode<K, V>** bucketArr;
    H hash;

    size_t getBucketIndex(const K key, const size_t bucketCount) const {
      return hash(key) % bucketCount;
    }

    bool insert(const K key, const V value, const bool upsert = false) {
      if (bucketArrHasReachedThreshold()) {
        inflateBucketArr();
      }
      itemCount++;
      size_t index = getBucketIndex(key, bucketCount);
      return addToBucket(this->bucketArr, index, key, value, upsert);
    }

    bool addToBucket(HashNode<K, V>** bucketArr, const size_t index, const K key, const V value, const bool upsert = false) {
      HashNode<K, V>* prevNode = nullptr;
      HashNode<K, V>* node = bucketArr[index];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node == nullptr) {
        node = new HashNode<K, V>(key, value);
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

    void clearBucket(HashNode<K, V>** bucketArr, const size_t index) {
      HashNode<K, V>* node = bucketArr[index];
      while (node != nullptr) {
        HashNode<K, V>* prevNode = node;
        node = node->getNext();
        delete prevNode;
      }
    }

    void clearBucketArr(HashNode<K, V>** bucketArr, const size_t bucketCount) {
      for (int index = 0; index < bucketCount; index++) {
        clearBucket(bucketArr, index);
      }
    }

    void inflateBucketArr() {
      const size_t newBucketCount = bucketCount * 2;
      HashNode<K, V>** newBucketArr = new HashNode<K, V>* [newBucketCount]();

      //cout << "inflating bucket array from " << bucketCount << " to " << newBucketCount << " now we have " << this->collisions << " collisions and " << this->itemCount << " items." << endl;

      for (int bucketArrIndex = 0; bucketArrIndex < bucketCount; bucketArrIndex++) {
        HashNode<K, V>* node = this->bucketArr[bucketArrIndex];
        while (node != nullptr) {
          size_t newBucketArrIndex = getBucketIndex(node->getKey(), newBucketCount);
          addToBucket(newBucketArr, newBucketArrIndex, node->getKey(), node->getValue());
          node = node->getNext();
        }
        clearBucket(bucketArr, bucketArrIndex);
      }

      delete[] this->bucketArr;
      this->bucketArr = newBucketArr;
      this->bucketCount = newBucketCount;
    }

    const bool bucketArrHasReachedThreshold() const {
      return this->itemCount >= this->bucketCount * 0.6;
    }
};

int main() {
  HashMap<unsigned, string> map;

  for (int i = 0; i < 30000000; i++) {
    map.set(i, "foo");
  }
  // cout << "4 equals " << map.get(4) << endl;
  // cout << "5 equals " << map.get(5) << endl;
  // cout << "there are " << map.count() << " items in the map" << endl;
  // cout << "deleting key 5" << endl;
  // map.remove(5);
  // cout << "there are " << map.count() << " items in the map" << endl;
  // cout << "4 equals " << map.get(4) << endl;
  // cout << "5 equals " << map.get(5) << endl;
  // //map.clear();
  // cout << "there are " << map.count() << " items in the map" << endl;
  // cout << "4 equals " << map.get(4) << endl;
  return 0;
}
