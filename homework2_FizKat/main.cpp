#include <iostream>
#include <string>
#include <utility>
#include <vector>

class InventoryItem {
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
  void print() const { std::cout << id_ << " " << name_ << describe() << '\n'; }
};

int main() {
  Helmet h1("H-1", "ya_mob_I_love_yandex");

  // attempting to show polymorphism
  InventoryItem& test_1 = h1;
  std::cout << test_1.inventoryId() << " " << test_1.name() << test_1.describe();
  return 0;
}