#include <vector>
#include <concepts>
#include <iostream>
#include <memory>
#include "exceptions.cpp"

using namespace std;
using namespace __gnu_cxx;

/**
 *
 * @tparam K generic type of keys held in node
 * @tparam V generic type of values held in node
 * @tparam k min-size of keys/values in node
 */
template <typename K , typename V, size_t k>
requires totally_ordered<K>
class BTreeNode {
  typedef unique_ptr<BTreeNode> node_ptr;
  /**
   * array of pairs containing keys and values
   */
  vector<pair<K, V>> entries;
  /**
   * array of pointers to each child node
   */
  vector<node_ptr> children;
  /**
   * boolean specifying if node is leaf or not
   */
  bool leaf;

public:

  /**
   * empty constructor setting leaf to true
   */
  BTreeNode(const bool isLeaf = true) : leaf(isLeaf), entries(), children() {};

  /**
   *
   * @return vector<pair<K, V>> pointer to the vector containing all entries for the node
   */
  const vector<pair<K, V>>& getEntries() const {
    return entries;
  }

  /**
   *
   * @return true if node is leaf false otherwise
   */
  bool isLeaf() const {
    return leaf;
  }

  /**
   *
   * @return vector<node_ptr>& pointer to the vector containing all pointers to
   * child-nodes
   */
  const vector<node_ptr>& getChildren() const {
    return children;
  }

  /**
   *
   * @return size_t size of node
   */
  size_t size() const {
    return entries.size();
  }

  /**
   *
   * @return 2 * k
   */
  static constexpr size_t maxSize() {
    return 2 * k;
  }

  /**
   *
   * @return bool true if node is full (size == 2k)
   */
  bool isFull() const {
    return size() == maxSize();
  }

  /**
   *
   * @param key K key to compare to
   * @return __gnu_cxx::__normal_iterator<const pair<K, V>*, vector<pair<K, V>>> iterator
   * to first position where the given key is smaller than or equal to another key in entries in node
   */
  //__gnu_cxx::__normal_iterator<const pair<K, V>*, vector<pair<K, V>>>
  __normal_iterator<const pair<K, V>*, vector<pair<K, V>>> findIndex(const K& key) const {
    return lower_bound(entries.begin(), entries.end(), key,
        [](const pair<K, V>& entry, const K& key) {
            return entry.first < key;
        });
  }

  /**
   *
   * @param key K key to insert into entries of node
   * @param value V value to insert into entries of node
   */
  void insert(const K& key, const V& value) {
    entries.emplace(findIndex(key), key, value);
  }

  void insert(const pair<K, V>& entry ) {
    insert(entry.first, entry.second);
  }

  /**
   *
   * @param child unique_ptr<BTreeNode> ptr to child node to insert into
   * children
   */
  void addChild(node_ptr child) {
    children.push_back(child);
  }

  /**
   *
   * @param it __gnu_cxx::__normal_iterator<const pair<K, V>*, vector<pair<K, V>>> iterator to first position where key is lower bound
   * @return size_t index of child node where key should be inserted
   */
  size_t findChildIndex(__normal_iterator<const pair<K, V>*, vector<pair<K, V>>> it) {
    return distance(entries.begin(), it);
  }

  pair<K, V> pop_at(size_t index) {
    if (index >= entries.size()) {
      throw out_of_range("Index out of range");
    }
    pair<K, V> entry = entries[index];
    entries.erase(entries.begin() + index);
    return entry;
  }

  /**
   * 
   * @param i size_t index of child to split
   */
  void  splitChild(size_t i) {
    //assert(child.entries.size == 2k+1);
    node_ptr child = children[i];
    insert(child->pop_at(k));
  }
};



//--------------------------------------------



template <typename K , typename V, size_t k>
requires totally_ordered<K>
class BTree {
  typedef unique_ptr<BTreeNode<K, V, k>> node_ptr;
  /**
   * root of BTree
   */
  node_ptr root;

public:
  BTree() : root(make_unique<BTreeNode<K, V, k>>()) {}

  /**
   *
   * @param key
   * @return
   */
  bool contains(const K& key) const {
    return contains_helper(root.get(), key);
  }

  /**
   *
   * @param key
   * @return
   */
  V get(const K& key) const {
    return get_helper(root.get(), key);
  }

  /**
   *
   * @param key
   * @param value
   */
  void insert(const K& key, const V& value) {
    if (contains(key)) {
      throw key_already_exists(to_string(key) + " already exists in tree");
    }
    insert_helper(root.get(), key, value);
  }

private:
  /**
   *
   * @param node
   * @param key
   * @return
   */
  bool contains_helper(const BTreeNode<K, V, k>* node, const K& key) const {
    auto it = node->findIndex(key);
    if (it != node->entries.end() && it->first == key) {
      return true;
    }
    if (node->leaf) {
      return false;
    }
    size_t childIndex = node->findChildIndex(it);
    return contains_helper(node->getChildren()[childIndex].get(), key);
  }

  /**
   *
   * @param node
   * @param key
   * @return
   */
  V get_helper(const BTreeNode<K, V, k>* node, const K& key) const {
    auto it = node->findIndex(key);
    if (it != node->entries.end() && it->first == key) {
      return it->second;
    }
    if (node->leaf) {
      throw key_not_in_tree(to_string(key) + " is not in tree");
    }
    size_t childIndex = node->findChildIndex(it);
    return get_helper(node->getChildren()[childIndex].get(), key);
  }

  /**
   *
   * @param node
   * @param key
   * @param value
   */
  void insert_helper(const BTreeNode<K, V, k>* node, const K& key, const V& value) {
    if (node->isLeaf()) {
      if (!node->isFull()) {
        node.insert(key, value);
      } else {
        //split node
        node.insert(key, value);
        if (node == root) {

        } else {

        }
      }
    } else {
      insert_helper(node->getChildren()[node->findChildIndex(node->findIndex(key))].get(), key, value);
    }
  }

};

int main() {
  return 0;
};
