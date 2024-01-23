#include <cstdio>
#include <thread>

#include <valgrind/callgrind.h>

struct cnt {
  volatile int a;
  volatile int b;
};

void inc_a(cnt &c) { c.a++; }
void inc_b(cnt &c) { c.b++; }

int main() {
  cnt C{0, 0};

  CALLGRIND_START_INSTRUMENTATION;

  std::thread T1([&C]() {
    for (int i = 0; i < 20000000; ++i) {
      inc_a(C);
    }
  });
  std::thread T2([&C]() {
    for (int i = 0; i < 10000000; ++i) {
      inc_b(C);
    }
  });
  T1.join();
  T2.join();

  CALLGRIND_STOP_INSTRUMENTATION;

  printf("%d %d\n", C.a, C.b);
  return 0;
}
