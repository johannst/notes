# c++

openstd [cpp standards][openstd-stds].

Source files of most examples is available [here][src-examples].

## Type deduction

Force compile error to see what `auto` is deduced to.
```cpp
auto foo = bar();

// force compile error
typename decltype(foo)::_;
```

## Strict aliasing and type punning

The `strict aliasing` rules describe via which `alias` a value can be accessed.
> Informal: an `alias` is a reference / pointer to a value.

Accessing a value through an alias that violates the strict aliasing rules is
`undefined behavior (UB)`.

Examples below on [godbolt](https://godbolt.org/z/TsvTY9zfj).
```cpp
int i = 0;

// Valid aliasing (signed / unsigned type).
*reinterpret_cast<signed int*>(&i);
*reinterpret_cast<unsigned int*>(&i);

// Valid aliasing (cv qualified type).
*reinterpret_cast<const int*>(&i);
*reinterpret_cast<const unsigned*>(&i);

// Valid aliasing (byte type).
*reinterpret_cast<char*>(&i);
*reinterpret_cast<std::byte*>(&i);

// Invalid aliasing, dereferencing pointer is UB.
*reinterpret_cast<short*>(&i);
*reinterpret_cast<float*>(&i);
```
> NOTE: Casting pointer to invalid aliasing type is not directly UB, but
> dereferencing the pointer is UB.

```cpp
short s[2] = { 1, 2 };

// Invalid aliasing (UB) - type punning, UB to deref ptr (int has stricter
// alignment requirements than short).
*reinterpret_cast<int*>(s);


// Arbitrary byte pointer.
char c[4] = { 1, 2, 3, 4 };

// Invalid aliasing (UB) - type punning, UB to deref ptr (int has stricter
// alignment requirements than char).
*reinterpret_cast<int*>(c);
```

At the time of writing, the current [c++ std draft][std-draft-aliasing]
contains the following.
```text
If a program attempts to access the stored value of an object through a glvalue
whose type is not **similar** (7.3.6) to one of the following types the
behavior is undefined [44]

(11.1) the dynamic type of the object,
(11.2) a type that is the signed or unsigned type corresponding to the dynamic
       type of the object, or
(11.3) a char, unsigned char, or std::byte type.

[44]: The intent of this list is to specify those circumstances in which an
      object can or cannot be aliased.
```

The paragraph is short but one also needs to understand the meaning of
[similar (*similar_types*)][std-draft-similar-types].

This paragraph is actually somewhat more explicit in the [c++17 std][std-17].
```text
If a program attempts to access the stored value of an object through a glvalue
of other than one of the following types the behavior is undefined [63]

(11.1) the dynamic type of the object,
(11.2) a cv-qualiﬁed version of the dynamic type of the object,
(11.3) a type similar (as deﬁned in 7.5) to the dynamic type of the object,
(11.4) a type that is the signed or unsigned type corresponding to the dynamic
       type of the object,
(11.5) a type that is the signed or unsigned type corresponding to a
       cv-qualiﬁed version of the dynamic type of the object,
(11.6) an aggregate or union type that includes one of the aforementioned types
       among its elements or non- static data members (including, recursively,
       an element or non-static data member of a subaggregate or contained
       union),
(11.7) a type that is a (possibly cv-qualiﬁed) base class type of the dynamic
       type of the object,
(11.8) a char, unsigned char, or std::byte type.

[63]: The intent of this list is to specify those circumstances in which an
      object may or may not be aliased.
```

Additional references:
- [What is the Strict Aliasing Rule and Why do we care][gist-strict-aliasing]

  The article shows a small example how the compiler may optimized using the
  strict aliasing rules.
  ```cpp
  int alias(int* i, char* c) {
    *i = 1;
    *c = 'a';  // char* may alias int*
    return *i;
  }

  int noalias(int* i, short* s) {
      *i = 1;
      *s = 2;  // short* does not alias int*
      return *i;
  }
  ```
  ```x86asm
  alias(int*, char*):
  mov    DWORD PTR [rdi] ,0x1  ; *i = 1;
  mov    BYTE PTR [rsi], 0x61  ; *c = 'a';
  mov    eax,DWORD PTR [rdi]   ; Must reload, char* can alias int*.
  ret

  noalias(int*, short*):
  mov    DWORD PTR [rdi], 0x1  ; *i = 1;
  mov    WORD PTR [rsi], 0x2   ; *s = 2;
  mov    eax,0x1               ; Must not reload, short* can not alias int*.
  ret
  ```
- [reinterpret_cast][reinterpret-aliasing] type aliasing
  > 5) Any object pointer type `T1*` can be converted to another object pointer
  > type `cv T2*`. This is exactly equivalent to `static_cast<cv
  > T2*>(static_cast<cv void*>(expression))` (which implies that if T2's
  > alignment requirement is not stricter than T1's, the value of the pointer
  > does not change and conversion of the resulting pointer back to its
  > original type yields the original value). In any case, the resulting
  > pointer may only be dereferenced safely if allowed by the type aliasing
  > rules (see below).

  ```cpp
  int I;
  char* X = reinterpret_cast<char*>(&I);  // Valid, char allowed to alias int.
  *X = 42;
  int* Y = reinterpret_cast<int*>(X);     // Cast back to original type.
  *Y = 1337;  // safe

  char C[4];
  int* P = reinterpret_cast<int*>(C);     // Cast is ok, not yet UB.
  *P = 1337; // UB, violates strict aliasing / alignment rules.
             // https://stackoverflow.com/questions/52492229/c-byte-array-to-int
  ```
 - On `gcc` strict aliasing is enabled starting with `-O2`.
   ```bash
   for i in {0..3} g s; do echo "-O$i $(g++ -Q --help=optimizers -O$i | grep fstrict-aliasing)"; done
   -O0   -fstrict-aliasing           [disabled]
   -O1   -fstrict-aliasing           [disabled]
   -O2   -fstrict-aliasing           [enabled]
   -O3   -fstrict-aliasing           [enabled]
   -Og   -fstrict-aliasing           [disabled]
   -Os   -fstrict-aliasing           [enabled]
   ```

### `__restrict` keyword

The `__restrict` keyword allows the programmer to tell the compiler that two
pointer will not alias each other.
```cpp
int alias(int* a, int* b) {
    *a = 1;
    *b = 2;
    return *a;
}

// alias(int*, int*):                           # @alias(int*, int*)
//         mov     dword ptr [rdi], 1
//         mov     dword ptr [rsi], 2
//         mov     eax, dword ptr [rdi]
//         ret

int noalias(int* __restrict a, int* __restrict b) {
    *a = 1;
    *b = 2;
    return *a;
}

// noalias(int*, int*):                         # @noalias(int*, int*)
//         mov     dword ptr [rdi], 1
//         mov     dword ptr [rsi], 2
//         mov     eax, 1
//         ret
```

However this should only be used with care and in a narrow scope, as it is easy
to violate self defined contract, see [godbolt](https://godbolt.org/z/e8x1af3Mh).

### Type punning

The correct way to do `type-punning` in c++:
1. [`std::bit_cast`][std-bitcast] (c++20)
1. [`std::memcpy`](https://godbolt.org/z/3PM4jGvEz)

## Variadic templates ([parameter pack][parameter-pack])

```cpp
{{#include c++/meta.cc:3:}}
```

## Forwarding reference ([fwd ref][fwd-ref])

A `forwarding reference` is a special references that preserves the `value
category` of a function parameter and therefore allows for `perfect`
forwarding.

A forwarding reference is a parameter of a function template, which is declared
as `rvalue` reference to a `non-cv` qualified `type` template parameter.
```cpp
template<typename T>
void fn(T&& param); // param is a forwarding reference
```

Perfect forwarding can be achieved with [`std::forward`][std-fwd]. This for
example allows a wrapper function to pass a parameter with the **exact** same
value category to a down-stream function which is being invoked in the wrapper.

```cpp
{{#include c++/fwd.cc:3:}}
```

## Example: `any_of` template meta function

```cpp
{{#include c++/meta3.cc:3:}}
```

## Example: [SFINAE][sfinae] ([enable_if][enable-if])

Provide a single entry point `Invoke` to call some `Operations`.
Use `enable_if` to enable/disable the template functions depending on the two
available traits an operation can have:
- Operation returns a result
- Operation requires a context

```cpp
{{#include c++/meta2.cc:3:}}
```

## Example: Minimal templatized test registry

A small test function registry bringing together a few different template
features.

```cpp
{{#include c++/meta4.cc:3:}}
```

## Example: Concepts pre c++20

Prior to c++20's concepts, `SFINAE` and `std::void_t` can be leveraged to build
something similar allowing to define an interface (aka trait) for a template
parameter.

```cpp
{{#include c++/concepts-11.cc:3:}}
```

The main mechanic can be explained with the following reduced example. If one
of the `decltype(std:declval<T>...` expressions is ill-formed, the template
specialization for `is_valid` will be removed from the candidate set due to
[SFINAE][sfinae].
```cpp
{{#include c++/tmpl-void_t.cc:3:45}}
```
> `std::declval<T>()` creates an instance of type T in an unevaluated context.

A more detailed description is available in the SO discussion [How does
`void_t` work](https://stackoverflow.com/a/27688405).

## Template selection with partially / fully specializations.
```cpp
{{#include c++/tmpl-pair.cc:3:}}
```

# Example: Perfect forwarding
```cpp
{{#include c++/fwd-perfect.cc:3:}}
```

[parameter-pack]: https://en.cppreference.com/w/cpp/language/parameter_pack
[enable-if]: https://en.cppreference.com/w/cpp/types/enable_if
[sfinae]: https://en.cppreference.com/w/cpp/language/sfinae
[fwd-ref]: https://en.cppreference.com/w/cpp/language/reference#Forwarding_references
[std-fwd]: https://en.cppreference.com/w/cpp/utility/forward
[std-bitcast]: https://en.cppreference.com/w/cpp/numeric/bit_cast
[src-examples]: https://github.com/johannst/notes/tree/master/src/development/c%2B%2B
[reinterpret-aliasing]: https://en.cppreference.com/w/cpp/language/reinterpret_cast#Type_aliasing
[gist-strict-aliasing]: https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8
[std-draft-aliasing]: http://eel.is/c++draft/basic.lval#11
[std-draft-similar-types]: http://eel.is/c++draft/conv.qual#def:similar_types
[std-17]: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
[openstd-home]: https://www.open-std.org/jtc1/sc22/wg21/
[openstd-stds]: https://www.open-std.org/jtc1/sc22/wg21/docs/standards
