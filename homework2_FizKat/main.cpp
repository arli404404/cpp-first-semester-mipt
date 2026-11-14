#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
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
  if (!std::isfinite(energy) || energy < 0) {
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
 private:
  std::shared_ptr<int> lifetime_ = std::make_shared<int>(0);

 public:
  virtual ~InventoryItem() = default;
  virtual std::string inventoryId() const = 0;
  virtual std::string name() const = 0;
  virtual std::string describe() const = 0;

  std::weak_ptr<int> lifetime() const { return lifetime_; }
};

std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
  return out << item.inventoryId() << " " << item.name();
}

bool operator<(const InventoryItem& l, const InventoryItem& r) {
  return l.inventoryId() < r.inventoryId();
}

bool operator==(const InventoryItem& a, const InventoryItem& b) {
  return a.inventoryId() == b.inventoryId();
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
  int score() const { return score_; }
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

class Cola : public InventoryItem {
 private:
  std::string id_;
  std::string name_;

 public:
  Cola(std::string id, std::string name)
      : id_(std::move(id)), name_(std::move(name)) {}

  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override { return "Cola"; }
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
  std::weak_ptr<int> focus_;
  std::string focusId_;
  bool hasFocus_ = false;

 public:
  void focus(const std::string& id) {
    for (const auto& item : items_) {
      if (item->inventoryId() == id) {
        focus_ = item->lifetime();
        focusId_ = id;
        hasFocus_ = true;
        return;
      }
    }

    throw ParkError("item not found");
  }
  void showFocus() const {
    if (!hasFocus_ || focus_.expired()) {
      std::cout << "removed / expired\n";
      return;
    }

    for (const auto& item : items_) {
      if (item->inventoryId() == focusId_) {
        std::cout << *item << '\n';
        return;
      }
    }

    std::cout << "removed / expired\n";
  }
  std::size_t countNovice() const {
    std::size_t count = 0;

    for (const auto& item : items_) {
      auto* vehicle = dynamic_cast<const Vehicle*>(item.get());

      if (vehicle != nullptr && vehicle->noviceFriendly()) {
        ++count;
      }
    }

    return count;
  }
  std::size_t size() const { return items_.size(); }
  void add(std::unique_ptr<InventoryItem> item) {
    if (!item) {
      throw ParkError("null item");
    }

    emptyId(item->inventoryId());

    for (const auto& existing : items_) {
      if (*existing == *item) {
        throw ParkError("duplicate ID");
      }
    }

    if (auto* vehicle = dynamic_cast<Vehicle*>(item.get())) {
      invalidScore(vehicle->score());
      inspect(vehicle->name());
    }

    if (auto* consumer = dynamic_cast<EnergyConsumer*>(item.get())) {
      invalidEnergy(consumer->dailyEnergy());
    }

    items_.add(std::move(item));
  }
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
  Park park;
  std::unordered_map<std::string, std::shared_ptr<ModelSpec>> specs;

  std::string line;

  while (std::getline(std::cin, line)) {
    std::istringstream input(line);
    std::string command;

    if (!(input >> command)) {
      continue;
    }

    try {
      auto checkEnd = [&input]() {
        std::string extra;
        if (input >> extra) {
          throw ParkError("too much argumentss");
        }
      };

      if (command == "quit") {
        checkEnd();
        break;
      }

      else if (command == "list") {
        checkEnd();
        park.list();
      }

      else if (command == "energy") {
        checkEnd();
        std::cout << std::fixed << std::setprecision(1) << park.energy()
                  << '\n';
      }

      else if (command == "novice") {
        checkEnd();
        park.novice();
      }

      else if (command == "count_novice") {
        checkEnd();
        std::cout << park.countNovice() << '\n';
      }

      else if (command == "size") {
        checkEnd();
        std::cout << park.size() << '\n';
      }

      else if (command == "show_focus") {
        checkEnd();
        park.showFocus();
      }

      else if (command == "focus") {
        std::string id;

        if (!(input >> id)) {
          throw ParkError("invalid arguments");
        }

        checkEnd();
        park.focus(id);
      }

      else if (command == "remove") {
        std::string id;

        if (!(input >> id)) {
          throw ParkError("invalid argument");
        }

        checkEnd();
        park.remove(id);
      }

      else if (command == "add") {
        std::string type, id, name;

        if (!(input >> type >> id >> name)) {
          throw ParkError("invalid argument");
        }

        if (type == "scooter") {
          double energy;
          int score;

          if (!(input >> energy >> score)) {
            throw ParkError("invalid argument");
          }

          checkEnd();
          auto it = specs.find(name);
          std::shared_ptr<ModelSpec> spec;

          if (it == specs.end()) {
            spec = std::make_shared<ModelSpec>(name);
          } else {
            spec = it->second;
          }

          park.add(std::make_unique<Scooter>(id, name, score, energy, spec));
          specs[name] = std::move(spec);
        }

        else if (type == "bike") {
          int score;

          if (!(input >> score)) {
            throw ParkError("invalid argument");
          }

          checkEnd();

          park.add(std::make_unique<Bike>(id, name, score));
        }

        else if (type == "ebike") {
          double energy;
          int score;

          if (!(input >> energy >> score)) {
            throw ParkError("invalid argument");
          }

          checkEnd();

          park.add(std::make_unique<EBike>(id, name, score, energy));
        }

        else if (type == "helmet") {
          checkEnd();

          park.add(std::make_unique<Helmet>(id, name));
        }

        else if (type == "dock") {
          double energy;

          if (!(input >> energy)) {
            throw ParkError("invalid argumenr");
          }

          checkEnd();

          park.add(std::make_unique<Dock>(id, name, energy));
        }

        else if (type == "cola") {
          checkEnd();

          park.add(std::make_unique<Cola>(id, name));
        }

        else {
          throw ParkError("unknown item");
        }
      }

      else {
        throw ParkError("no such command");
      }
    } catch (const std::exception& error) {
      std::cout << "error: " << error.what() << '\n';
    }
  }

  return 0;
}
