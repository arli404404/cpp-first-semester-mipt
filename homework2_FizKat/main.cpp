#include <iostream>
#include <string>
#include <utility>
#include <vector>

class InventoryItem {
 private:
  std::string id_;
  std::string name_;

 public:
  virtual ~InventoryItem() = default;
  virtual std::string inventoryId() const = 0;
  virtual std::string name() const = 0;
  virtual std::string describe() const = 0;
};

class Helmet : public InventoryItem {
 private:
  std::string id_;
  std::string name_;
 public:
  Helmet(std::string id, std::string name)
      : id_(std::move(id)), name_(std::move(name)) {}
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override { return "Helmet"; }
  void print() const { std::cout << id_ << " " << name_ << "Helmet" << '\n'; }
  // А можно как-то в print выводить не просто "Helmet", а именно значение Helmet.desrcibe()?
};

int main() {
  Helmet test_1("H-1", "ya_mob_I_love_yandex");
  Helmet test_2("H-2", "2::413");
  test_1.print();
  test_2.print();
  return 0;
}