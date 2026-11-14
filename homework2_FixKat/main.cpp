#include <iostream>
#include <utility>
#include <vector>
#include <string>


class Helmet {
  private:
    std::string id_;
    std::string name_;
  public:
    std::string getId() {
      return id_;
    }
    std::string getName() {
      return name_;
    }
    void print() {
      std::cout << id_ << " " << name_;
    }
};