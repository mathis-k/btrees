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
  BTreeNode(const bool isLeaf = true) : entries(), children(), leaf(isLeaf) {};

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
   * @param isLeaf bool to set leaf to
   */
  void setLeaf(const bool isLeaf) {
    leaf = isLeaf;
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
   * @return true if node is overflowing
   */
  bool isOverflowing() const {
    return size() > maxSize();
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

  /**
   *
   * @param entry pair of key and value to insert into btree as const reference
   */
  void insert(const pair<K, V>& entry ) {
    insert(entry.first, entry.second);
  }

  /**
   *
   * @param child unique_ptr<BTreeNode> ptr to child node to insert into
   * children
   */
  void addChild(node_ptr child) {
    children.push_back(move(child));
  }

  /**
   *
   * @param it __gnu_cxx::__normal_iterator<const pair<K, V>*, vector<pair<K, V>>> iterator to first position where key is lower bound
   * @return size_t index of child node where key should be inserted
   */
  size_t findChildIndex(const __normal_iterator<const pair<K, V>*, vector<pair<K, V>>> it) const {
    return distance(entries.begin(), it);
  }

  /**
   *
   * @param index size_t index of element to pop from entries
   * @return pair of key and value of element at given index
   */
  pair<K, V> pop_at(size_t index) {
    if (index >= entries.size()) {
      throw out_of_range("Index out of range in pop_at()");
    }
    pair<K, V> entry = entries[index];
    entries.erase(entries.begin() + index);
    return entry;
  }

  /**
   *
   * @param from vector from which to move entries
   * @param to vector into which entries should be moved
   * @param index starting index in from
   */
  template <typename T>
  void move_entries(vector<T>* from, vector<T>* to, size_t index) {
    if (index >= from->size()) {
      throw out_of_range("Start index out of range in move_entries()");
    }
    to->insert(to->end(),
               std::make_move_iterator(from->begin() + index),
               std::make_move_iterator(from->end()));
    from->erase(from->begin() + index, from->end());
  }

    /**
   *
   * @param i size_t index of child to split
   */
  void  splitChild(size_t i) {
    node_ptr child = move(children[i]);
    if (!child->isOverflowing()) {
      throw logic_error("Cannot split a non-full child node");
    }

    insert(child->pop_at(k));

    node_ptr newNode = make_unique<BTreeNode>(child->isLeaf());

    move_entries(&child->entries, &newNode->entries, k);
    if (!child->isLeaf()) {
      move_entries(&child->children, &newNode->children, k + 1);
    }

    children.insert(children.begin() + i + 1, move(newNode));
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
   * @param key const K& reference of key to lookup in tree
   * @return true if key exists, false otherwise
   */
  bool contains(const K& key) const {
    return contains_helper(root.get(), key);
  }

  /**
   *
   * @param key const K& reference of key of value to get
   * @return V value for given key in tree
   */
  V get(const K& key) const {
    return get_helper(root.get(), key);
  }

  /**
   *
   * @param key const K& reference of key to insert into tree
   * @param value const V& reference of value to insert into tree with given key
   * @throws key_already_exists if given key already exists in node
   */
  void insert(const K& key, const V& value) {
    if (contains(key)) {
      throw key_already_exists(to_string(key) + " already exists in tree");
    }
    if (root->isLeaf()) {
      insert_root(key, value);
    } else {
      insert_helper(root.get(), key, value);
    }
  }

  /**
   *
   */
  void printTree() const {
    printInOrder(root.get());
  }

private:
  /**
   *
   * @param node BTreeNode<K, V, k>* pointer to current node
   * @param const K& reference of key to lookup
   * @return true if node contains key
   */
  bool contains_helper(const BTreeNode<K, V, k>* node, const K& key) const {
    auto it = node->findIndex(key);
    if (it != node->getEntries().end() && it->first == key) {
      return true;
    }
    if (node->isLeaf()) {
      return false;
    }
    size_t childIndex = node->findChildIndex(it);
    return contains_helper(node->getChildren()[childIndex].get(), key);
  }

  /**
   *
   * @param node BTreeNode<K, V, k>* pointer to current node
   * @param key const K& reference of key for value to get
   * @return V value for given tree
   * @throws key_not_in_tree if given key is not in tree
   */
  V get_helper(const BTreeNode<K, V, k>* node, const K& key) const {
    auto it = node->findIndex(key);
    if (it != node->getEntries().end() && it->first == key) {
      return it->second;
    }
    if (node->isLeaf()) {
      throw key_not_in_tree(to_string(key) + " is not in tree");
    }
    size_t childIndex = node->findChildIndex(it);
    return get_helper(node->getChildren()[childIndex].get(), key);
  }

  /**
   *
   * @param key const K& reference of key to insert
   * @param value const V& reference of value to insert with given key
   */
  void insert_root(const K& key, const V& value) {
    //assert(root->isLeaf);
    root->insert(key, value);
    if (root->isOverflowing()) {
      node_ptr newNode = make_unique<BTreeNode<K, V, k>>(false);
      newNode->addChild(move(root));
      root = move(newNode);
      root->splitChild(0);
    }
  }

  /**
   *
   * @param node BTreeNode<K, V, k>* pointer to current node
   * @param key const K& reference of key to insert
   * @param value const V& reference of value to insert with given key
   */
  void insert_helper(BTreeNode<K, V, k>* node, const K& key, const V& value) {
    if (node->isLeaf()) {
      node->insert(key, value);
    } else {
      size_t childIndex = node->findChildIndex(node->findIndex(key));
      BTreeNode<K, V, k>* child = node->getChildren()[childIndex].get();
      insert_helper(child, key, value);
      if (child->isOverflowing()) {
        node->splitChild(childIndex);
      }
    }
  }

  /**
   *
   * @param node BTreeNode<K, V, k>* pointer to current node
   */
  void printInOrder(const BTreeNode<K, V, k>* node) const {
    if (!node) return;
    for (size_t i = 0; i < node->size(); ++i) {
      if (!node->isLeaf()) {
        printInOrder(node->getChildren()[i].get());
      }
      cout << node->getEntries()[i].first << ": " << node->getEntries()[i].second << endl;
    }
    if (!node->isLeaf()) {
      printInOrder(node->getChildren().back().get());
    }
  }
};

int main() {
  constexpr size_t k = 2;
  BTree<int, string, k> tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(5, "five");
  tree.insert(6, "six");
  tree.insert(8, "eight");

  tree.printTree();

  return 0;
};
