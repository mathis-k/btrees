#include <vector>
#include <concepts>
#include <iostream>
#include <memory>
#include "exceptions.cpp"

using namespace std;

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

  size_t findChildIndex(typename vector<pair<K, V>>::iterator it) {
    return distance(entries.begin(), it);
  }
};




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

private:
  /**
   *
   * @param node
   * @param key
   * @return
   */
  bool contains_helper(const BTreeNode<K, V, k>* node, const K& key) const {
    if (node == nullptr) {
      return false;
    }
    auto it = lower_bound(node->entries.begin(), node->entries.end(), key,
        [](const pair<K, V>& entry, const K& key) {
            return entry.first < key;
        });
    if (it != node->entries.end() && it->first == key) {
      return true;
    }
    if (node->leaf) {
      return false;
    }
    size_t childIndex = node->findChildIndex(it);
    return contains_helper(node->children[childIndex].get(), key);
  }

  /**
   *
   * @param node
   * @param key
   * @return
   */
  V get_helper(const BTreeNode<K, V, k>* node, const K& key) const {
    if (node == nullptr) {
      throw key_not_in_tree(key + " is not in the tree");
    }
    auto it = lower_bound(node->entries.begin(), node->entries.end(), key,
        [](const pair<K, V>& entry, const K& key) {
            return entry.first < key;
        });
    if (it != node->entries.end() && it->first == key) {
      return it->second;
    }
    if (node->leaf) {
      throw key_not_in_tree(key + " is not in the tree");
    }
    size_t childIndex = node->findChildIndex(it);
    return get_helper(node->children[childIndex].get(), key);
  }
};

int main() {
  return 0;
};
