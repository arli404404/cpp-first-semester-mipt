#include <algorithm>
#include <iostream>
#include <memory>
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

std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
  return out << item.inventoryId() << " " << item.name();
}

bool operator<(const InventoryItem& l, const InventoryItem& r) {
  return l.inventoryId() < r.inventoryId();
}

// Vehicle classes

constexpr int newbieFriendlyScore = 6;

class Vehicle : public InventoryItem {
 protected:
  std::string id_;
  std::string name_;
  int score_;

 public:
  Vehicle(std::string id, std::string name, int score)
      : id_(std::move(id)), name_(std::move(name)), score_(std::move(score)) {}
  virtual bool noviceFriendly() const { return score_ >= newbieFriendlyScore; }
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
};

class EnergyConsumer {
 protected:
  double energy_;

 public:
  virtual ~EnergyConsumer() = default;
  EnergyConsumer(double energy) : energy_(energy) {}
  virtual double dailyEnergy() const = 0;
};

class Bike : public Vehicle {
 public:
  Bike(std::string id, std::string name, int score)
      : Vehicle(id, name, score) {}
  std::string describe() const override { return "Bike"; }

  void print() const {
    std::cout << id_ << " " << name_ << " " << describe() << " "
              << noviceFriendly() << '\n';
  }
};

class Scooter : public Vehicle, public EnergyConsumer {
 public:
  Scooter(std::string id, std::string name, int score, double energy)
      : Vehicle(id, name, score), EnergyConsumer(energy) {}
  std::string describe() const override { return "Scooter"; }
  double dailyEnergy() const override { return energy_; }

  void print() const {
    std::cout << id_ << " " << name_ << " " << describe() << " "
              << noviceFriendly() << " " << dailyEnergy() << '\n';
  }
};

class EBike : public Vehicle, public EnergyConsumer {
 public:
  EBike(std::string id, std::string name, int score, double energy)
      : Vehicle(id, name, score), EnergyConsumer(energy) {}
  std::string describe() const override { return "Electric bike"; }
  double dailyEnergy() const override { return energy_; }

  void print() const {
    std::cout << id_ << " " << name_ << " " << describe() << " "
              << noviceFriendly() << " " << dailyEnergy() << '\n';
  }
};

// Items and Dock classes

class Dock : public InventoryItem, public EnergyConsumer {
 private:
  std::string id_;
  std::string name_;

 public:
  Dock(std::string id, std::string name, double energy)
      : EnergyConsumer(energy), id_(std::move(id)), name_(std::move(name)) {}
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override { return "Dock"; }
  double dailyEnergy() const override { return energy_; }
  void print() const {
    std::cout << id_ << " " << name_ << " " << describe() << " "
              << dailyEnergy() << " " << '\n';
  }
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
  void print() const {
    std::cout << id_ << " " << name_ << " " << describe() << '\n';
  }
};

// created a new type for a vector
template <typename T>
class Reg {
 private:
  std::vector<T> info_;

 public:
  void add(T value) { info_.push_back(std::move(value)); }
  auto begin() const { return info_.begin(); }

  auto end() const { return info_.end(); }
  std::size_t size() const { return info_.size(); }
  T& operator[](std::size_t i) { return info_[i]; }
};

class Park {
 private:
  Reg<std::unique_ptr<InventoryItem>> items_;
  std::vector<const InventoryItem*> sorted;

 public:
  std::size_t size() const { return items_.size(); }
  void add(std::unique_ptr<InventoryItem> item) { items_.add(std::move(item)); }
  void list() const {
    for (const auto& item : items_) {
      std::cout << *item << "\n";
    }
  }
  double energy() const {
    double total = 0;
    for (const auto& item : items_) {
      auto* consumer = dynamic_cast<EnergyConsumer*>(item.get());

      if (consumer != nullptr) {
        total += consumer->dailyEnergy();
      }
    }
    return total;
  }
  void novice() const {
    for (const auto& item : items_) {
      auto* vehicle = dynamic_cast<Vehicle*>(item.get());

      if (vehicle != nullptr) {
        bool friendliness = vehicle->noviceFriendly();
        if (friendliness) {std::cout << *item << "\n";}
      }
    }
  }
};

int main() {
  Helmet h1("H-1", "ya_mob_I_love_yandex");

  // attempting to show polymorphism
  InventoryItem& test_1 = h1;
  std::cout << test_1.inventoryId() << " " << test_1.name() << " "
            << test_1.describe() << '\n';

  // added vehicles
  Bike bike1("B-1", "Wrooom", 0);
  Scooter scooter1("S-1", "Turbo67", 6, 2.0);
  EBike ebike1("E-1", "Pro", 7, 1.0);

  bike1.print();
  scooter1.print();
  ebike1.print();

  Park park;
  park.add(std::make_unique<Helmet>("H-2", "ya_mob_I_love_T_Bank"));
  park.add(std::make_unique<Scooter>("S-2", "Turbo67", 6, 2.0));
  park.add(std::make_unique<Dock>("D-1", "SuperDock", 1.5));
  park.list();
  std::cout << park.size() << "\n";
  std::cout << park.energy() << "\n";
  return 0;
}