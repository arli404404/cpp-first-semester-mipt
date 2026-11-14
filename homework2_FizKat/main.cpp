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

constexpr int newbieFriendlyScore = 6;

class Vehicle : public InventoryItem {
 protected:
  std::string id_;
  std::string name_;
  int score_;

 public:
  Vehicle(std::string id, std::string name, int score)
      : id_(std::move(id)), name_(std::move(name)), score_(std::move(score)) {}
  virtual bool noviceFriendly() const {
    return score_ >= newbieFriendlyScore;
  }
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
};

class Bike : public Vehicle {
 public:
  Bike(std::string id, std::string name, int score)
      : Vehicle(id, name, score) {}
  std::string describe() const override { return "Bike"; }
};

class Scooter : public Vehicle {
 private:
  double energy_;

 public:
  Scooter(std::string id, std::string name, int score, double energy)
      : Vehicle(id, name, score), energy_(energy) {}
  std::string describe() const override { return "Scooter"; }
  double dailyEnergy() const { return energy_; }
};

class EBike : public Vehicle {
 private:
  double energy_;

 public:
  EBike(std::string id, std::string name, int score, double energy)
      : Vehicle(id, name, score), energy_(energy) {}
  std::string describe() const override { return "Electric bike"; }
  double dailyEnergy() const { return energy_; }
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
  std::cout << test_1.inventoryId() << " " << test_1.name() << " "
            << test_1.describe();

  // added vehicles
  Bike bike1("B-1", "wrooom", 0);
  Scooter scooter1("S-1", "Turbo67", 6, 2.0);
  EBike ebike("B-1", "Pro", 7, 1.0);
  return 0;
}