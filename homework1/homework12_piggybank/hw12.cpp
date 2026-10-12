#include <cstring>
#include <iostream>

using Func = void (*)(int&, int&);

void deposit(int& balance, int& status) {
  int num;
  std::cin >> num;
  if (num <= 0) {
    std::cout << "error\n";
  } else {
    balance += num;
  }
}
void withdraw(int& balance, int& status) {
  int num;
  std::cin >> num;
  if (num > balance || num <= 0) {
    std::cout << "error\n";
  } else {
    balance -= num;
  }
}
void balance(int& balance, int& status) { std::cout << balance << "\n"; }
void reset(int& balance, int& status) { balance = 0; }
void stop(int& balance, int& status) { status = 0; }

// Doubles the amount of money in the account if balance != 67 and balance <
// 1000; If balance == 67 or balance >= 1000, the stock market crashes and
// balance sets to 0
void invest(int& balance, int& status) {
  if (balance != 67 && balance < 1000) {
    balance *= 2;
    std::cout << "stock market prices grew up\n";
  } else {
    balance = 0;
    std::cout << "stock market crashed\n";
  }
}

struct Command {
  const char* name;
  Func func;
};

Command commands[] = {{"deposit", deposit},
                      {"withdraw", withdraw},
                      {"balance", balance},
                      {"reset", reset},
                      {"invest", invest}};

const int COMMANDS_NUM = sizeof(commands) / sizeof(commands[0]);

int main() {
  int status = 1;
  int bal = 0;
  char cmd[32];
  while (status == 1 && (std::cin >> cmd)) {
    int correct = 0;
    if (std::strcmp(cmd, "stop") == 0) {
      status = 0;
      break;
    }
    for (int i = 0; i < COMMANDS_NUM; i++) {
      if (std::strcmp(cmd, commands[i].name) == 0) {
        commands[i].func(bal, status);
        correct = 1;
        break;
      }
    }
    if (correct == 0) {
      std::cout << "error\n";
    }
  }
  return 0;
}