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
```
btrees/
├── CMakeLists.txt
├── README.md
├── exceptions.cpp
├── btree.cpp
└── main.cpp
```
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
- **Efficient Insertion**: The tree handles node splits automatically when nodes become full.
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

## Examples

### 1. **Inserting Entries into the BTree**

```cpp
#include <iostream>
#include "btree.cpp"

int main() {
    constexpr size_t k = 2;
    BTree<int, std::string, k> tree;

    // Insert values into the B-tree
    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(30, "thirty");

    // Print the tree in-order
    tree.print();

    return 0;
};
```

### 2. **Updating Entries**

```cpp
#include <iostream>
#include "btree.cpp"

int main() {
    constexpr size_t k = 2;
    BTree<int, std::string, k> tree;

    // Insert values into the B-tree
    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(30, "thirty");

    // Update the value for key 20
    tree.insert(20, "updated twenty");

    // Print the tree in-order
    tree.print();

    return 0;
};
```

### 3. **Searching for Entries**

```cpp
#include <iostream>
#include "btree.cpp"

int main() {
    constexpr size_t k = 2;
    BTree<int, std::string, k> tree;

    // Insert values into the B-tree
    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(30, "thirty");

    // Search for a key
    int key = 20;
    if (tree.contains(key)) {
        std::cout << "Found key " << key << " with value: " << tree.get(key) << std::endl;
    } else {
        std::cout << "Key " << key << " not found!" << std::endl;
    }

    return 0;
};
```

### 4. **Handling Non-Existent Keys**

```cpp
#include <iostream>
#include "btree.cpp"

int main() {
    constexpr size_t k = 2;
    BTree<int, std::string, k> tree;

    // Insert values into the B-tree
    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(30, "thirty");

    // Try to search for a non-existant key
    try {
        int key = 40;
        std::cout << "Value for key " << key << ": " << tree.get(key) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;  // Erwartete Ausnahme
    }

    return 0;
};
```

### 5. **Big Example**

```cpp
#include <iostream>
#include "btree.cpp"

int main() {
  constexpr size_t k = 2;  // Min-Size = 2, Max-Size = 4
  BTree<int, std::string, k> tree;

  // Very large set of entries for testing
  std::vector<std::pair<int, std::string>> values = {
    {50, "fifty"}, {20, "twenty"}, {70, "seventy"}, {10, "ten"},
    {30, "thirty"}, {60, "sixty"}, {80, "eighty"}, {5, "five"},
    {15, "fifteen"}, {25, "twenty-five"}, {35, "thirty-five"}, {55, "fifty-five"},
    {65, "sixty-five"}, {75, "seventy-five"}, {85, "eighty-five"}, {1, "one"},
    {7, "seven"}, {12, "twelve"}, {18, "eighteen"}, {28, "twenty-eight"},
    {22, "twenty-two"}, {33, "thirty-three"}, {44, "forty-four"}, {57, "fifty-seven"},
    {63, "sixty-three"}, {77, "seventy-seven"}, {90, "ninety"}, {100, "hundred"},
    {110, "one hundred ten"}, {120, "one hundred twenty"}, {130, "one hundred thirty"},
    {140, "one hundred forty"}, {150, "one hundred fifty"}, {160, "one hundred sixty"},
    {170, "one hundred seventy"}, {180, "one hundred eighty"}, {190, "one hundred ninety"},
    {200, "two hundred"}, {210, "two hundred ten"}, {220, "two hundred twenty"},
    {230, "two hundred thirty"}, {240, "two hundred forty"}, {250, "two hundred fifty"},
    {260, "two hundred sixty"}, {270, "two hundred seventy"}, {280, "two hundred eighty"},
    {290, "two hundred ninety"}, {300, "three hundred"}, {310, "three hundred ten"},
    {320, "three hundred twenty"}, {330, "three hundred thirty"}, {340, "three hundred forty"},
    {350, "three hundred fifty"}, {360, "three hundred sixty"}, {370, "three hundred seventy"},
    {380, "three hundred eighty"}, {390, "three hundred ninety"}, {400, "four hundred"}
  };

  // Inserting values into the tree
  for (const auto& [key, value] : values) {
    tree.insert(key, value);
  }

  // Printing the tree after insertion (In-Order Traversal)
  std::cout << "After inserting values:" << std::endl;
  tree.print();

  // Updating an existing key (key 1)
  tree.insert(1, "updated one");
  std::cout << "\nAfter updating key 1:" << std::endl;
  tree.print();

  // Searching for an existing key (key 25)
  int keyToFind = 25;
  if (tree.contains(keyToFind)) {
    std::cout << "\nFound key " << keyToFind << " with value: " << tree.get(keyToFind) << std::endl;
  } else {
    std::cout << "\nKey " << keyToFind << " not found!" << std::endl;
  }

  // Searching for a non-existent key (key 1000)
  int nonExistentKey = 1000;
  try {
    std::cout << "\nValue for non-existent key " << nonExistentKey << ": " << tree.get(nonExistentKey) << std::endl;
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  // Inserting more values to test overflow and node splitting
  tree.insert(500, "five hundred");
  tree.insert(600, "six hundred");
  tree.insert(700, "seven hundred");
  tree.insert(800, "eight hundred");
  tree.insert(900, "nine hundred");

  std::cout << "\nAfter inserting more values and updating existing keys:" << std::endl;
  tree.print();

  // Inserting another key and testing overflow handling
  tree.insert(1000, "thousand");

  std::cout << "\nAfter inserting key 1000:" << std::endl;
  tree.print();

  // The tree will overflow and need to split nodes when too many entries are inserted.
  // This process can be observed in the tree output when it is printed again.

  return 0;
};
```