# c++

## Type deduction

Force compile error to see what `auto` is deduced to.
```cpp
auto foo = bar();

// force compile error
typename decltype(foo)::_;
```

