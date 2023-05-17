// Copyright (C) 2023 johannst

#include <cstdio>

void tell_me(int desc) {
  if (desc & 1) {
    std::puts("this");
  } else {
    std::puts("that");
  }
}

int main(int argc, char *argv[]) {
  tell_me(argc);
  tell_me(argc);
  return 0;
}
