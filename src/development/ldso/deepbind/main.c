#include <dlfcn.h>
#include <stdio.h>

int main() {
  puts("-- deep --");
  // Load library into its own LOCAL scope with DEEPBINDING, meaning that
  // symbols will first resolve to the library + its dependencies first before
  // considering global symbols.
  void* h1 = dlopen("./libdeep.so", RTLD_LAZY | RTLD_LOCAL | RTLD_DEEPBIND);
  if (h1) {
    // Lookup symbol in the libraries LOCAL scope (library + its own
    // dependencies).
    void (*test_fn)() = dlsym(h1, "test");
    test_fn();

    // Lookup non-existing symbol in the libraries LOCAL scope (library + its
    // own dependencies).
    (void)dlsym(h1, "libdeep_main");
  }

  puts("-- nodp --");
  // Load library into its own LOCAL scope.
  void* h2 = dlopen("./libnodp.so", RTLD_LOCAL | RTLD_LAZY);
  if (h2) {
    // Lookup symbol in the libraries LOCAL scope (library + its own
    // dependencies).
    void (*test_fn)() = dlsym(h2, "test");
    test_fn();

    // Lookup non-existing symbol in the libraries LOCAL scope (library + its
    // own dependencies).
    (void)dlsym(h2, "libnodp_main");
  }

  puts("-- main --");
  // Lookup non-existing symbol in the GLOBAL scope.
  (void)dlsym(RTLD_DEFAULT, "default_main");

  return 0;
}
