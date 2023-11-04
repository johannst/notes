# cmake(1)

## `PRIVATE` / `PUBLIC` / `INTERFACE`

These modifier control where properties for a given target are visible.

- `PRIVATE`: Only for the target itself.
- `INTERFACE`: Only for anyone linking against the target.
- `PUBLIC`: For the target itself and anyone linking against it (effectively
  `PRIVATE` + `INTERFACE`).

The following gives an example for preprocessor definitions specified on a
library target. This behaves in the same way for other properties like for
example include directories.
```cmake
# CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(moose)

# -- LIBRARY
add_library(liba STATIC liba.cc)
target_compile_definitions(liba PUBLIC DEF_PUBLIC)
target_compile_definitions(liba PRIVATE DEF_PRIVATE)
target_compile_definitions(liba INTERFACE DEF_INTERFACE)

# -- APPLICATION
add_executable(main main.cc)
target_link_libraries(main liba)
```

```sh
> touch liba.cc; echo "int main() {}" > main.cc
> cmake -B build -S . -G Ninja
> ninja -C build -j1 --verbose
[1/4] /usr/bin/c++ -DDEF_PRIVATE -DDEF_PUBLIC  [..] .../liba.cc
[2/4] [..]
[3/4] /usr/bin/c++ -DDEF_INTERFACE -DDEF_PUBLIC [..] .../main.cc
[4/4] [..]
```
