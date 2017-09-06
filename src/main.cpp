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
    HashMap(size_t bucketCount = 1000000) {
      if (bucketCount == 0) {
        bucketCount = 1;
      }
      this->itemCount = 0;
      this->bucketCount = bucketCount;
      this->buckets = new HashNode<K, V> * [bucketCount]();
    }

    V get(const K key) {
      size_t bucketIndex = getBucketIndex(key);
      HashNode<K, V> * node = buckets[bucketIndex];

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
      size_t bucketIndex = getBucketIndex(key);
      HashNode<K, V> * prevNode = nullptr;
      HashNode<K, V> * node = buckets[bucketIndex];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node == nullptr) {
        node = new HashNode<K, V>(key, value);
        if (prevNode == nullptr) {
          buckets[bucketIndex] = node;
        } else {
          prevNode->setNext(node);
        }

        itemCount++;
      } else {
        node->setValue(value);
      }

      return true;
    }

    bool remove(const K key) {
      size_t bucketIndex = getBucketIndex(key);
      HashNode<K, V> * prevNode = nullptr;
      HashNode<K, V> * node = buckets[bucketIndex];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node && node->getKey() == key) {
        if (prevNode != nullptr) {
          prevNode->setNext(node->getNext());
        } else {
          buckets[bucketIndex] = nullptr;
        }

        delete node;
        itemCount--;

        return true;
      }

      return false;
    }

    size_t count() {
      return itemCount;
    }

  private:
    size_t itemCount;
    size_t bucketCount;
    HashNode<K, V> * * buckets;
    H hash;

    size_t getBucketIndex(const K key) {
      return hash(key) % bucketCount;
    }
};

int main() {
  HashMap<unsigned long int, string> map;

  for (int i = 0; i < 10; i++) {
    map.set(i, "foo" + to_string(i));
  }

  cout << "4 equals " << map.get(4) << endl;
  cout << "5 equals " << map.get(5) << endl;
  cout << "6 equals " << map.get(6) << endl;
  cout << "there are " << map.count() << " items in the map" << endl;
  map.remove(5);
  cout << "there are " << map.count() << " items in the map" << endl;
  cout << "4 equals " << map.get(4) << endl;
  cout << "5 equals " << map.get(5) << endl;
  cout << "6 equals " << map.get(6) << endl;

  return 0;
}
