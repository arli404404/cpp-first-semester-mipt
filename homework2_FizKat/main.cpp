#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Helmet {
 private:
  std::string id_;
  std::string name_;

 public:
  Helmet(std::string id, std::string name) : id_(id), name_(name) {}
  std::string getId() const { return id_; }
  std::string getName() const { return name_; }
  void print() const { std::cout << id_ << " " << name_ << '\n'; }
};