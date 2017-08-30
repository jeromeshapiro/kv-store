#include <iostream>
#include <string>

using namespace std;

template<typename K, typename V>
class HashItem {
  public:
    HashItem(const K key, const V value) {
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

    HashItem * getNext() {
      return next;
    }

    void setNext(HashItem<K, V> * item) {
      this->next = item;
    }

  private:
    K key;
    V value;
    HashItem * next;
};

template<typename K, typename V, typename H = hash<K> >
class HashMap {
  public:
    HashMap(size_t bucketSize = 16) {
      this->bucketSize = bucketSize;
      this->bucket = new HashItem<K, V> * [bucketSize]();
    }

    bool set(const K key, const V value) {
      size_t hashVal = getBucketIndex(key);
      HashItem<K, V> * prevItem = nullptr;
      HashItem<K, V> * item = bucket[hashVal];

      while (item != nullptr && item->getKey() != key) {
        prevItem = item;
        item = item->getNext();
      }

      if (item == nullptr) {
        item = new HashItem<K, V>(key, value);
        if (prevItem == nullptr) {
          bucket[hashVal] = item;
        } else {
          prevItem->setNext(item);
        }
      } else {
        item->setValue(value);
      }

      return true;
    }

    V get(const K key) {
      size_t hashVal = getBucketIndex(key);
      HashItem<K, V> * item = bucket[hashVal];
      
      while (item != nullptr) {
        if (item->getKey() == key) {
          return item->getValue();
        } else {
          item = item->getNext();
        }
      }
      
      V empty;
      return empty;
    }

  private:
    size_t bucketSize;
    HashItem<K, V> * * bucket;
    H hash;

    size_t getBucketIndex(const K key) {
      return hash(key) % bucketSize;
    }
};

int main() {
  HashMap<int, string> map(1000000);

  for (int i = 0; i < 1000000; i++) {
    map.set(i, "foo");
    map.get(i);
  }

  return 0;
}

