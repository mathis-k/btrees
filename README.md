# B-Tree Implementation in C++

This repository contains an implementation of a **B-Tree**, a self-balancing tree data structure commonly used in databases and filesystems to store sorted data and allow for efficient insertion, deletion, and search operations.

## Table of Contents
1. [Introduction](#introduction)
2. [Structure](#structure)
3. [How It Works](#how-it-works)
4. [Features](#features)
5. [Installation](#installation)
6. [Usage](#usage)
7. [Examples](#examples)

## Introduction
A **B-Tree** is a generalization of a binary search tree where each node can have more than two children. The tree is balanced, which ensures that the operations like searching, inserting, and deleting are efficient even with large datasets.

In this implementation, the tree nodes store key-value pairs and are able to handle overflow by splitting nodes when they exceed their maximum capacity.

## Structure

### BTreeNode

The `BTreeNode` class represents a single node of the B-tree. Each node can have:

- A list of key-value pairs (`entries`).
- A list of child node pointers (`children`).
- A boolean `leaf` to indicate whether the node is a leaf node.

The class provides functions for:
- Inserting key-value pairs.
- Searching for entries.
- Splitting a node when it becomes full.
- Managing child nodes.

### BTree

The `BTree` class manages the overall tree structure. It includes:
- Insertion of key-value pairs into the tree.
- Searching for a value associated with a given key.
- Printing the tree in an in-order fashion.

## How It Works

### Node Structure

Each node in the B-tree contains:
- A vector of `entries`, where each entry is a key-value pair (`pair<K, V>`).
- A vector of `children`, where each child is a pointer to another `BTreeNode`.

### Insertion

The insertion process works as follows:
1. **Find the correct position**: Traverse down the tree to the appropriate leaf node, comparing keys as necessary.
2. **Insert into the node**: If the node is not full, insert the key-value pair into the sorted list of entries.
3. **Handle overflow**: If the node becomes full (i.e., it contains `2*k` entries), the node splits. The median value is pushed up to the parent node, and the entries are redistributed between the two new nodes.

### Splitting Nodes

When a node is full (contains `2*k` entries), it must be split:
- The median element is moved to the parent node.
- The child node is split into two, and the children of the node are reallocated.

### Searching

Searching for a key is a recursive process:
1. The function traverses down the tree comparing keys in the current node.
2. If the key is found, the associated value is returned.
3. If the key is not found, the function moves to the appropriate child node and continues the search.

## Features
- **Balanced Tree**: The B-tree maintains its balance, ensuring that all operations have logarithmic time complexity.
- **Efficient Insertion and Deletion**: The tree handles node splits automatically when nodes become full.
- **Key-Value Storage**: The tree stores key-value pairs, supporting both retrieval and updates.

## Installation

### Prerequisites

To build and run this project, you need:
- A C++20 compatible compiler (e.g., GCC 10+ or Clang).
- CMake (version 3.10 or higher).

### Cloning the Repository

To clone this repository and build the project, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/mathis-k/btrees.git
2. Navigate to the project directory:
    ```bash
   cd btrees
3. Create a build directory:
    ```bash
   mkdir build
   cd build
4. Generate the build files using CMake:
    ```bash
   cmake ..
5. Build the project:
    ```bash
   cmake --build .

### Running the Program
Once the project is built, you can run the executable to test the B-tree implementation:
```bash
  ./bin/btrees
```
## Usage

### Insert Key-Value Pairs

To insert key-value pairs into the tree, use the `insert` method:
```cpp
tree.insert(50, "fifty");
tree.insert(20, "twenty");
tree.insert(70, "seventy");
```

## Search for a Key

To search for a key and retrieve its value:
```cpp
string value = tree.get(50);  // Returns "fifty"
```

## Print the Tree

You can print the B-tree in an in-order fashion:
```cpp
tree.print();
```

## Example

Here’s an example of how to use the B-tree:
```cpp
int main() {
  constexpr size_t k = 2;
  BTree<int, string, k> tree;

  vector<pair<int, string>> values = {
    {50, "fifty"}, {20, "twenty"}, {70, "seventy"},
    {10, "ten"}, {30, "thirty"}, {60, "sixty"}
  };

  // Insert values into the B-tree
  for (const auto& [key, value] : values) {
    tree.insert(key, value);
  }

  // Print the tree in-order
  cout << "In-Order Traversal of B-tree:" << endl;
  tree.print();

  // Insert a new value and print again
  tree.insert(40, "forty");
  tree.print();

  return 0;
}
```