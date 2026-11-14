#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Helmet {
 private:
  std::string id_;
  std::string name_;

 public:
  Helmet(std::string id, std::string name)
      : id_(std::move(id)), name_(std::move(name)) {}
  std::string getId() const { return id_; }
  std::string getName() const { return name_; }
  void print() const { std::cout << id_ << " " << name_ << '\n'; }
};

int main() {
  Helmet test_1("H-1", "ya_mob_I_love_yandex");
  Helmet test_2("H-2", "2::413");
  test_1.print();
  test_2.print();
  return 0;
};