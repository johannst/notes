# gcc(1)

## CLI

### Preprocessing
While debugging can be helpful to just pre-process files.

```bash
gcc -E [-dM] ...
```
- `-E` run only preprocessor
- `-dM` list only `#define` statements

## [Builtins][builtins]

### `__builtin_expect(expr, cond)`
Give the compiler a hint which branch is hot, so it can lay out the code
accordingly to reduce number of jump instructions.
See on [compiler explorer](https://godbolt.org/z/MbTHAP).

```bash
echo "
extern void foo();
extern void bar();
void run0(int x) {
  if (__builtin_expect(x,0)) { foo(); }
  else { bar(); }
}
void run1(int x) {
  if (__builtin_expect(x,1)) { foo(); }
  else { bar(); }
}
" | gcc -O2 -S -masm=intel -o /dev/stdout -xc -
```

Will generate something similar to the following.
- `run0`: `bar` is on the path without branch
- `run1`: `foo` is on the path without branch
```c
run0:
        test    edi, edi
        jne     .L4
        xor     eax, eax
        jmp     bar
.L4:
        xor     eax, eax
        jmp     foo
run1:
        test    edi, edi
        je      .L6
        xor     eax, eax
        jmp     foo
.L6:
        xor     eax, eax
        jmp     bar
```

[builtins]: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
