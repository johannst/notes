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

## [SFINAE][sfinae] example ([enable_if][enable-if])

```cpp
{{#include c++/meta2.cc:3:}}

```

[gist-strict-asliasing]: https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8
[parameter-pack]: https://en.cppreference.com/w/cpp/language/parameter_pack
[enable-if]: https://en.cppreference.com/w/cpp/types/enable_if
[sfinae]: https://en.cppreference.com/w/cpp/language/sfinae
