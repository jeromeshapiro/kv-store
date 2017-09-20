#ifndef HASHMAPNODE_H_
#define HASHMAPNODE_H_

#include <memory>

namespace kvmap {
  template<typename K, typename V>
  class HashMapNode {
    typedef std::unique_ptr<HashMapNode<K, V>> nextNode;
    public:
      HashMapNode(const K key, const V val) : _key(key), _val(val), _next(nullptr) {}

      K getKey() const { return _key; }

      V getValue() const { return _val; }

      void setValue(const V val) { _val = val; }

      HashMapNode* getNext() const { return _next; }

      void setNext(HashMapNode<K, V>* node) { _next = node; _nextNode = node; }

    private:
      K _key;
      V _val;
      HashMapNode* _next;
      nextNode _nextNode;
  };
}

#endif /* HASHNODE_H_ */
