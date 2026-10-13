#include <iostream>
#include <random>

/*
 * Задача: Система управления контингентом университета с возможностью
 * мгновенного отчисления студентов и целых подразделений.
 *
 * Для чего тут int***: Модель данных "Университет -> Факультет -> Группа ->
 * Студент". Три уровня указателей позволяют хранить группы в разных участках
 * памяти и мгновенно (за O(1)) отчислять целую группу через delete[], не трогая
 * остальных.
 *
 * Зачем void*: С помощью его я удаляю группы. Перед удалением группы
 * применяется "безопасное затирание памяти". Он принимает void* и затирает
 * данные по байтам, так как ему не важен тип хранимой информации.
 *
 * Далее комментарии делал на английском, потому что посчитал, что так
 * правильнее
 */

int main() {
  int*** journal;
  int FACULTIES_COUNT = 3;
  int GROUPS_IN_FACULTY = 2;
  int STUDENTS_IN_GROUP = 4;
  journal = new int**[FACULTIES_COUNT];

  for (int f = 0; f < FACULTIES_COUNT; f++) {
    journal[f] = new int*[GROUPS_IN_FACULTY];
    for (int g = 0; g < GROUPS_IN_FACULTY; g++) {
      journal[f][g] = new int[STUDENTS_IN_GROUP];
      for (int s = 0; s < STUDENTS_IN_GROUP; s++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(100, 999);
        journal[f][g][s] = distrib(gen);
      }
    }
  }

  std::cout << "created a journal\n";

  // The array contains information about faculties. Each one of faculties
  // contains a few groups of a few student IDs each;
  // Before we can expel a student, we should give them an ID:

  journal[0][0][0] = 415, journal[0][0][1] = 623, journal[0][0][2] = -1,
  journal[0][0][3] = 584;

  journal[0][1][0] = 452, journal[0][1][1] = 129, journal[0][1][2] = 387,
  journal[0][1][3] = 706;

  // If an ID of a student equals -1, that means the student has been expelled;
  // Expulsion of a 3th (fourth) student in 1st (second) group of a 0th (first)
  // faculty works like this:

  journal[0][1][3] = -1;

  std::cout << "expelled a student\n";

  // Now let me show you how to expel an entire group in O(1)! :)

  void* raw = journal[0][1];  // A pointer for the group we want to expel
  unsigned char* wiper = static_cast<unsigned char*>(
      raw);  // Turning our group into a list of bytes

  int count_wipe = STUDENTS_IN_GROUP *
                   sizeof(int);  // Before we can delete journal[0][1], we
                                 // should clean up the data of it first
  for (int i = 0; i < count_wipe; i++) {
    wiper[i] = 0;
  }

  delete[] journal[0][1];
  journal[0][1] = nullptr;

  std::cout << "a group has been expelled\n";

  // Now that we've done our job of expelling an entire group, let's expel an
  // entire university and finish our program!

  for (int f = 0; f < FACULTIES_COUNT; f++) {
    for (int g = 0; g < GROUPS_IN_FACULTY; g++) {
      if (journal[f][g] != nullptr) {
        delete[] journal[f][g];
      }
    }
    delete[] journal[f];
  }

  delete[] journal;

  std::cout << "everything's ok\n";

  return 0;
}