#include <iomanip>
#include <iostream>

double avg_temp(const int* temps, const int n) {
  double sum = 0;
  for (int i = 0; i < n; i++) {
    sum += temps[i];
  }
  return sum / n;
}

void min_max_temp(const int* temps, const int n, int& min_val, int& max_val) {
  if (n > 0) {
    min_val = temps[0], max_val = temps[0];
    for (int i = 0; i < n; i++) {
      if (temps[i] < min_val) {
        min_val = temps[i];
      }
      if (temps[i] > max_val) {
        max_val = temps[i];
      }
    }
  }
}

int count_frosty_days(const int* temps, const int n) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (temps[i] < 0) {
      count += 1;
    }
  }
  return count;
}

int main() {
  int n;
  std::cin >> n;
  int* temps = new int[n];

  int min_val, max_val;
  for (int i = 0; i < n; i++) {
    std::cin >> temps[i];
  }
  std::cout << std::fixed << std::setprecision(1) << avg_temp(temps, n) << '\n';
  min_max_temp(temps, n, min_val, max_val);
  std::cout << min_val << ' ' << max_val << '\n';
  std::cout << count_frosty_days(temps, n) << '\n';

  delete[] temps;
  return 0;
}