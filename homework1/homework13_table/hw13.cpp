#include <cstring>
#include <iomanip>
#include <iostream>

const int ROWS = 5;
const int COLS = 10;
char board[ROWS][COLS];

bool put(int x, int y, char c) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
    return false;
  } else {
    board[y][x] = c;
    return true;
  }
}

bool text(int x, int y, const char* c) {
  int len = std::strlen(c);
  if (x < 0 || x + len > COLS || y < 0 || y >= ROWS) {
    return false;
  } else {
    for (int i = x; i < x + len; i++) {
      board[y][i] = c[i - x];
    }
    return true;
  }
}

void clear() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      board[i][j] = ' ';
    }
  }
}

void print() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      std::cout << board[i][j];
    }
    std::cout << "\n";
  }
}

int main() {
  char cmd[16];
  while (std::cin >> cmd) {
    if (strcmp(cmd, "quit") == 0)
      break;
    else if (strcmp(cmd, "clear") == 0)
      clear();
    else if (strcmp(cmd, "print") == 0)
      print();
    else if (strcmp(cmd, "put") == 0) {
      int x, y;
      char c;
      std::cin >> x >> y >> c;
      if (!put(x, y, c)) std::cout << "error\n";
    } else if (strcmp(cmd, "text") == 0) {
      int x, y;
      char word[11];
      std::cin >> x >> y >> std::setw(11) >> word;
      if (!text(x, y, word)) std::cout << "error\n";
    } else {
      std::cout << "error\n";
    }
  }
  return 0;
}