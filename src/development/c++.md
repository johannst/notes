# c++

## Type deduction

Force compile error to see what `auto` is deduced to.
```cpp
auto foo = bar();

// force compile error
typename decltype(foo)::_;
```

## Variadic templates ([parameter pack][parameter-pack])

```cpp
{{#include c++/meta.cc:3:}}
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
template<typename T, typename = void>
struct is_valid : std::false_type {};

template<typename T>
struct is_valid<T, std::void_t<
                       decltype(std::declval<T>().some_fun1()),
                       decltype(std::declval<T>().some_fun2())
               >> : std::true_type {};
```
> `std::declval<T>()` creates an instance of type T in an unevaluated context.


[gist-strict-asliasing]: https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8
[parameter-pack]: https://en.cppreference.com/w/cpp/language/parameter_pack
[enable-if]: https://en.cppreference.com/w/cpp/types/enable_if
[sfinae]: https://en.cppreference.com/w/cpp/language/sfinae
