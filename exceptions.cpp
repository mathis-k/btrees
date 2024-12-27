#include <exception>
#include <string>
class key_not_in_tree : public std::exception {
private:
  std::string message;
public:
  explicit key_not_in_tree(const std::string& msg) : message(msg) {}

  const char* what() const noexcept override {
    return message.c_str();
  }
};

class node_full : public std::exception {
private:
  std::string message;
public:
  explicit node_full(const std::string& msg) : message(msg) {}

  const char* what() const noexcept override {
    return message.c_str();
  }
};