#include <vector>
#include <concepts>
#include <iostream>
#include <memory>

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

public:
  /**
   * boolean specifying if node is leaf or not
   */
  bool leaf;

  /**
   * empty constructor setting leaf to true
   */
  BTreeNode(const bool isLeaf = true) : leaf(isLeaf), entries(), children() {};

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
  constexpr size_t maxSize() const {
    return 2 * k;
  }
  
  /**
   *
   * @param key key to look for in node
   * @return true if key is stored in node false otherwise
   */
  bool contains (const K& key) const {
    return any_of(entries.begin(), entries.end(),
        [&key](const pair<K, V>& entry) {
            return entry.first == key;
        });
  }

  /**
   *
   * @param key key of value that should be returned in node
   * @return T value of key stored in node
   */
  V get (const K& key) const {
    auto it = lower_bound(entries.begin(), entries.end(), key,
            [](const pair<K, V>& entry, const K& key) {
                return entry.first < key;
            });
    if (it != entries.end() && it->first == key) {
      return it->second;
    }
    throw out_of_range("Key not found");
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
   * @param key key of value to insert in node
   * @param value value to insert in node
   */
  void add(const K& key, const V& value) {
    if (isFull()) {
      throw overflow_error("Node is full");
    }
    auto it = lower_bound(entries.begin(), entries.end(), key,
        [](const pair<K, V>& entry, const K& key) {
            return entry.first < key;
        });
    entries.insert(it, make_pair(key, value));
  }

  size_t findChildIndex(typename vector<pair<K, V>>::iterator it) {
    return distance(entries.begin(), it);
  }

  friend class BTree;
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

  bool contains(node_ptr node = root, const K& key) {
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
    return contains(node->children[childIndex].get(), key);
  }
};

int main() {
  return 0;
};
