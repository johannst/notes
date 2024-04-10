#include <stdio.h>

#ifndef NAME
#define NAME __FILE_NAME__
#endif

void moose() {
  puts(NAME " says moose");
}
