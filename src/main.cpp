#include <vector>
#include <iostream>
#include <string>

using namespace std;

template<typename K, typename V>
class HashItem {
  public:
    HashItem(K key, V value) {
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

    void setValue(V value) {
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

int main() {
  HashItem<string, string> * item1 = new HashItem<string, string>("1", "test1");
  HashItem<string, string> * item2 = new HashItem<string, string>("2", "test2");

  item1->setNext(item2);

  cout << item1->getValue() << endl;
  cout << item1->getNext()->getValue() << endl;
  return 0;
}

