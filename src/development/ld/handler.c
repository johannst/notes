#include <stdio.h>

#define SECTION(S) __attribute__((section(S)))
#define USED       __attribute__((used))

// -- FUNCTON POINTER TABLE ----------------------------------------------------

typedef void (*fntab_t)();

// Define a function table entry with a priotiy.
#define FNTAB_ENTRY(E, P) \
  SECTION(".fntab" #P) USED static const fntab_t fnentry_##E##P = E;

// Iterate the function table.
#define FNTAB_FOREACH(V)    \
  extern char _fntab_start; \
  extern char _fntab_end;   \
  for (fntab_t* V = (fntab_t*)&_fntab_start; V < (fntab_t*)&_fntab_end; ++V)

// func10 & func11 are defined with the same prority, order not guaranteed.
// func20 has a lower priority, only runs after func10 & func11.

void func10() {
  puts("func10 called");
}
FNTAB_ENTRY(func10, 1);

void func11() {
  puts("func11 called");
}
FNTAB_ENTRY(func11, 1);

void func20() {
  puts("func20 called");
}
FNTAB_ENTRY(func20, 2);

// -- DATA TABLE ---------------------------------------------------------------

struct datatab_t {
  const char* name;
  void (*fn)();
};

#define DATATAB_ENTRY(E) \
  SECTION(".datatab")    \
  USED static const struct datatab_t dentry_##E = {.name = #E, .fn = E};

#define DATATAB_FOREACH(V)                                               \
  extern char _datatab_start;                                            \
  extern char _datatab_end;                                              \
  for (struct datatab_t* V = (struct datatab_t*)&_datatab_start; \
       V < (struct datatab_t*)&_datatab_end; ++V)

DATATAB_ENTRY(func10);
DATATAB_ENTRY(func20);

// -- RUN EXAMPLE --------------------------------------------------------------

int main() {
  FNTAB_FOREACH(f) {
    printf("call fntab entry @%p\n", *f);
    (*f)();
  }

  DATATAB_FOREACH(d) {
    printf("datatab entry d->name=%s call d->fn=%p\n", d->name, d->fn);
    (*d->fn)();
  }
}
