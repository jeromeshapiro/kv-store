#ifndef HASHMAPNODE_H_
#define HASHMAPNODE_H_

namespace kvmap {
  template<typename K, typename V>
  class HashMapNode {
    public:
      HashMapNode(const K key, const V value) {
        this->key = key;
        this->value = value;
        this->next = nullptr;
      }

      K getKey() const {
        return this->key;
      }

      V getValue() const {
        return this->value;
      }

      void setValue(const V value) {
        this->value = value;
      }

      HashMapNode* getNext() const {
        return this->next;
      }

      void setNext(HashMapNode<K, V>* node) {
        this->next = node;
      }

    private:
      K key;
      V value;
      HashMapNode* next;
  };
}

#endif /* HASHNODE_H_ */
