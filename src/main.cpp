#include <vector>
#include <iostream>
#include <string>

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

template<typename K, typename V, typename H = std::hash<K> >
class HashMap {
  public:
    HashMap(std::size_t bucketSize = 16) {
      this->bucketSize = bucketSize;
      this->bucket = new HashItem<K, V> * [bucketSize]();
    }

    bool put(const K key, const V value) {
      std::cout << getBucketIndex(key) << std::endl;
      return true;
    }
  private:
    std::size_t bucketSize;
    HashItem<K, V> * * bucket;
    H hash;

    std::size_t getBucketIndex(const K key) {
      return hash(key) % bucketSize;
    }
};

int main() {
  HashItem<int, std::string> * item1 = new HashItem<int, std::string>(1, "test1");
  HashItem<int, std::string> * item2 = new HashItem<int, std::string>(2, "test2");

  HashMap<std::string, std::string> * map = new HashMap<std::string, std::string>();

  map->put("foo", "test1");

  item1->setNext(item2);

  std::cout << item1->getValue() << std::endl;
  std::cout << item1->getNext()->getValue() << std::endl;
  return 0;
}

