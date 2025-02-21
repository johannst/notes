#include <ittnotify.h>

void init();
void compute();
void shutdown();

int main() {
  init();

  __itt_resume();
  compute();
  __itt_pause();

  shutdown();
  return 0;
}

//

#include <unistd.h>

void init() {
  sleep(1);
}

void compute() {
  sleep(1);
}

void shutdown() {
  sleep(1);
}
