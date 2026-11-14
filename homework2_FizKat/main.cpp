#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Classes for errors I wanna sleep
class ParkError : public std::runtime_error {
 public:
  ParkError(const std::string& m) : std::runtime_error(m) {}
};

class InspectionRejected : public ParkError {
 public:
  InspectionRejected() : ParkError("inspection rejected") {}
};

class NegativeEnergy : public ParkError {
 public:
  NegativeEnergy() : ParkError("negrative energy") {}
};

class EmptyId : public ParkError {
 public:
  EmptyId() : ParkError("empty ID") {}
};

class InvalidScore : public ParkError {
 public:
  InvalidScore() : ParkError("invalid score") {}
};

void inspect(const std::string& name) {
  std::string n = name;
  std::transform(n.begin(), n.end(), n.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (n.find("broken") != std::string::npos) {
    throw InspectionRejected();
  }
}

void invalidEnergy(double energy) {
  if (energy < 0) {
    throw NegativeEnergy();
  }
}

void emptyId(std::string id) {
  if (id == "") {
    throw EmptyId();
  }
}

void invalidScore(int score) {
  if (score < 1 || score > 10) {
    throw InvalidScore();
  }
}

// designing Inventory

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

class ModelSpec {
 public:
  std::string modelName;

  ModelSpec(std::string name) : modelName(std::move(name)) {}
};

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
 private:
  std::shared_ptr<ModelSpec> spec_;

 public:
  Scooter(std::string id, std::string name, int score, double energy,
          std::shared_ptr<ModelSpec> spec = nullptr)
      : Vehicle(id, name, score),
        EnergyConsumer(energy),
        spec_(std::move(spec)) {}
  std::string describe() const override { return "Scooter"; }
  double dailyEnergy() const override { return energy_; }

  void print() const {
    std::cout << id_ << " " << name_ << " " << describe() << " "
              << noviceFriendly() << " " << dailyEnergy() << '\n';
  }
  std::string modelName() const { return spec_ ? spec_->modelName : name_; }
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
  template <typename C>
  void removeIf(C condition) {
    info_.erase(std::remove_if(info_.begin(), info_.end(), condition),
                info_.end());
  }
};

class Park {
 private:
  Reg<std::unique_ptr<InventoryItem>> items_;

 public:
  std::size_t size() const { return items_.size(); }
  void add(std::unique_ptr<InventoryItem> item) { items_.add(std::move(item)); }
  void list() const {
    std::vector<const InventoryItem*> sorted;
    for (const auto& item : items_) {
      sorted.push_back(item.get());
    }
    std::sort(
        sorted.begin(), sorted.end(),
        [](const InventoryItem* a, const InventoryItem* b) { return *a < *b; });
    for (const auto* item : sorted) {
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
        if (friendliness) {
          std::cout << *item << "\n";
        }
      }
    }
  }
  void remove(const std::string& id) {
    items_.removeIf(
        [&id](const auto& item) { return item->inventoryId() == id; });
  }
};

int main() {
  Helmet h1("H-1", "ya_mob_I_love_yandex");

  // attempting to show polymorphism
  InventoryItem& test_1 = h1;
  std::cout << test_1.inventoryId() << " " << test_1.name() << " "
            << test_1.describe() << '\n';

  // added vehicles

  Park park;
  /*try {
      auto* consumer = dynamic_cast<EnergyConsumer*>(item.get());
      auto* vehicle = dynamic_cast<Vehicle*>(item.get());

      if (consumer != nullptr) {
        invalidEnergy(consumer->dailyEnergy());
      }
      if (vehicle != nullptr) {
        inspect(vehicle->name());
      }

    } catch (const std::exception& error) {
      std::cout << "error: " << error.what() << '\n';
    }*/
  try {
    emptyId("S-1");
    inspect("brokenX");
    invalidScore(8);
    invalidEnergy(2.5);
    park.add(std::make_unique<Scooter>("S-1", "brokenX", 8, 2.5));
  } catch (const std::exception& error) {
    std::cout << "error: " << error.what() << '\n';
  }
  park.add(std::make_unique<Helmet>("H-2", "ya_mob_I_love_T_Bank"));
  try {
    emptyId("S-1");
    inspect("Turbo69420");
    invalidScore(6);
    invalidEnergy(-1.0);
    park.add(std::make_unique<Scooter>("S-0", "Turbo69420", 6, -1.0));
  } catch (const std::exception& error) {
    std::cout << "error: " << error.what() << '\n';
  }
  park.add(std::make_unique<Scooter>("S-2", "Turbo67", 6, 2.0));
  park.add(std::make_unique<Dock>("D-1", "SuperDock", 1.5));
  park.list();
  std::cout << park.size() << "\n";
  std::cout << park.energy() << "\n";
  park.remove("S-2");

  auto commonSpec = std::make_shared<ModelSpec>("Turbo67Pro");
  park.add(std::make_unique<Scooter>("S-3", "Turbo67Pro", 6, 2.0, commonSpec));
  park.add(std::make_unique<Scooter>("S-4", "Turbo67Pro", 7, 1.75, commonSpec));
  std::cout << commonSpec.use_count() << "\n";
  std::cout << park.size() << "\n";
  std::cout << park.energy() << "\n";
  return 0;
}