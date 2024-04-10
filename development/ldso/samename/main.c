#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

void print_libso() {
  FILE* m = fopen("/proc/self/maps", "r");
  assert(m);

  char line[256];
  while (fgets(line, sizeof(line), m)) {
    if (strstr(line, "lib.so")) {
      printf("%s", line);
    }
  }
  fclose(m);
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    void* h = dlopen(argv[i], RTLD_LAZY | RTLD_GLOBAL);
    if (!h) {
      puts(dlerror());
      return 1;
    }

    void (*next)() = dlsym(h, "moose");
    assert(next);
    next();

    // leak lib, we want to priint the mmap.
  }

  print_libso();
  return 0;
}
