#include <unistd.h>

void work3() {
  usleep(3 * 1000);
}

void work2() {
  usleep(5 * 1000);
  work3();
}

void work1() {
  usleep(10 * 1000);
}

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; ++i) {
    work1();
    work2();
  }
}
