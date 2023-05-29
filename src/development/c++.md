# c++

Source files of most examples is available [here][src-examples].

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

[gist-strict-asliasing]: https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8
[parameter-pack]: https://en.cppreference.com/w/cpp/language/parameter_pack
[enable-if]: https://en.cppreference.com/w/cpp/types/enable_if
[sfinae]: https://en.cppreference.com/w/cpp/language/sfinae
[fwd-ref]: https://en.cppreference.com/w/cpp/language/reference#Forwarding_references
[std-fwd]: https://en.cppreference.com/w/cpp/utility/forward
[src-examples]: https://github.com/johannst/notes/tree/master/src/development/c%2B%2B
