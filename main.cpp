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
}
