# gcov(1)

Generate code coverage reports in text format.

Compile the source files of interest and link the final binary with the
following flags:
- `-fprofile-arcs` instruments the generated code such that it writes a `.gcda`
  file when being executed with details about which branches are taken
- `-ftest-coverage` writes a `.gcno` notes file which is used by `gcov` during
  generation of the coverage report

Depending on the build environment one may also set `-fprofile-abs-path` to
generate absolute path names into the `.gcno` note files, this can ease setups
where compilations are done in different directories to the source directory.

> `gcc` / `clang` also support an alias flag `--coverage` which during
> compilation time is equivalent to `-fprofile-arcs -ftest-coverage` and during
> link time `-lgcov`.

After running the instrumented binary, the human readable report can then be
generated for a single file for example such as
```shell
gcov <SRC FILE | OBJ FILE>
```

## Example
```cpp
{{#include gcov/cov.cc:3:}}
```

The `gcov` coverage report can be generated as follows for `gcc` or `clang`.
```make
{{#include gcov/Makefile:3:}}
```

The will generate a report similar to the following.
```text
cat cov.cc.gcov
        -:    0:Source:cov.cc
        -:    0:Graph:cov.gcno
        -:    0:Data:cov.gcda
        -:    0:Runs:1
        -:    1:// Copyright (C) 2023 johannst
        -:    2:
        -:    3:#include <cstdio>
        -:    4:
        2:    5:void tell_me(int desc) {
        2:    6:  if (desc & 1) {
        2:    7:    std::puts("this");
        -:    8:  } else {
    #####:    9:    std::puts("that");
        -:   10:  }
        2:   11:}
        -:   12:
        1:   13:int main(int argc, char *argv[]) {
        1:   14:  tell_me(argc);
        1:   15:  tell_me(argc);
        1:   16:  return 0;
        -:   17:}
```
