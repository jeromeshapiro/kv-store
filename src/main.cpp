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

    ~HashNode() {
      cout << "deleting node" << endl;
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
    HashMap(size_t bucketSize = 16) {
      this->bucketSize = bucketSize;
      this->bucket = new HashNode<K, V> * [bucketSize]();
    }

    bool set(const K key, const V value) {
      size_t bucketIndex = getBucketIndex(key);
      HashNode<K, V> * prevNode = nullptr;
      HashNode<K, V> * node = bucket[bucketIndex];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (node == nullptr) {
        node = new HashNode<K, V>(key, value);
        if (prevNode == nullptr) {
          bucket[bucketIndex] = node;
        } else {
          prevNode->setNext(node);
        }
      } else {
        node->setValue(value);
      }

      return true;
    }

    V get(const K key) {
      size_t bucketIndex = getBucketIndex(key);
      HashNode<K, V> * node = bucket[bucketIndex];

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

    bool remove(const K key) {
      size_t bucketIndex = getBucketIndex(key);
      HashNode<K, V> * prevNode = nullptr;
      HashNode<K, V> * node = bucket[bucketIndex];

      while (node != nullptr && node->getKey() != key) {
        prevNode = node;
        node = node->getNext();
      }

      if (prevNode != nullptr) {
        prevNode->setNext(node->getNext());
      } else {
        bucket[bucketIndex] = nullptr;
      }
      delete node;

      return false;
    }

  private:
    size_t bucketSize;
    HashNode<K, V> * * bucket;
    H hash;

    size_t getBucketIndex(const K key) {
      return hash(key) % bucketSize;
    }
};

int main() {
  HashMap<int, string> map(1000000);

  for (int i = 0; i < 10; i++) {
    map.set(i, "foo" + to_string(i));
  }

  cout << "4 equals " << map.get(4) << endl;
  cout << "5 equals " << map.get(5) << endl;
  cout << "6 equals " << map.get(6) << endl;
  map.remove(5);
  cout << "4 equals " << map.get(4) << endl;
  cout << "5 equals " << map.get(5) << endl;
  cout << "6 equals " << map.get(6) << endl;

  return 0;
}

