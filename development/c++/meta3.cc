// Copyright (C) 2022 johannst

#include <type_traits>

template<typename T, typename... U>
struct any_of : std::false_type {};

// Found our type T in the list of types U.
template<typename T, typename... U>
struct any_of<T, T, U...> : std::true_type {};

// Pop off the first element in the list of types U,
// since it didn't match our type T.
template<typename T, typename U0, typename... U>
struct any_of<T, U0, U...> : any_of<T, U...> {};

// Convenience template variable to invoke meta function.
template<typename T, typename... U>
constexpr bool any_of_v = any_of<T, U...>::value;

static_assert(any_of_v<int, char, bool, int>, "");
static_assert(!any_of_v<int, char, bool, float>, "");
