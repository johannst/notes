#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

#ifndef NAME
#define NAME ""
#endif

void test() {
  puts(NAME ":test()");

  // Lookup next symbol from the libraries scope, which will search only in the
  // libraries LOCAL scope, starting from the next object after the current one.
  (void)dlsym(RTLD_NEXT, "next_lib" NAME);

  // Global lookup from the libraries scope, which will search libraries in the
  // GLOBAL scope and the libraries LOCAL scope. The order in which the scopes
  // are searched depends on whether the library was loaded (a) with DEEPBIND or
  // (b) not. whether this library was loaded with DEEPBIND). In the first case,
  // the LOCAL scope is searched first, where in the latter, the GLOBAL scope is
  // searched first.
  (void)dlsym(RTLD_DEFAULT, "default_lib" NAME);
}
