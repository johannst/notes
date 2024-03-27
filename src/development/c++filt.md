# c++filt(1)

## Demangle symbol
```markdown
  c++-filt [opts] <symbol_str>
    -t    Try to also demangle types.
```

## Demangle stream
For example dynamic symbol table:
```markdown
  readelf -W --dyn-syms <elf> | c++filt
```

## Demangle types
```c++
// file: type.cc
#include <cstdio>
#include <typeinfo>

#define P(ty) printf(#ty " -> %s\n", typeid(ty).name())

template <typename T = void>
struct Foo {};

int main() {
  P(int);
  P(unsigned char);
  P(Foo<>);
  P(Foo<int>);
}
```
Build and run:
```sh
$ clang++ type.cc && ./a.out | c++filt
int -> i
unsigned char -> h
Foo<> -> 3FooIvE
Foo<int> -> 3FooIiE

$ clang++ type.cc && ./a.out | c++filt -t
int -> int
unsigned char -> unsigned char
Foo<> -> Foo<void>
Foo<int> -> Foo<int>
```
