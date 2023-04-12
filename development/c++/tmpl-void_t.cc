// Copyright (C) 2023 johannst

#include <type_traits>

// (1) Primary template.
template<typename T, typename = void>
struct is_valid : std::false_type {};

// (2) Partial template specialization.
template<typename T>
struct is_valid<T, std::void_t<decltype(std::declval<T>().some_fun1()),
                               decltype(std::declval<T>().some_fun2())>> : std::true_type {};
struct A {
    void some_fun1() {}
    void some_fun2() {}
};

struct B {};

static_assert(is_valid<A>::value, "is true");
// * Compare template arg list with primary template, we only supplied one
//   arg, the second one will be defaulted as
//   is_valid<A, void>
// * Compare template arg list against available specializations, this will
//   try to match the pattern <A, void> against the patterns defined in the
//   partial specializations.
// * Try specialization (2)
//   * T -> A
//   * Evaluate std::void_t -> decltype's are well-formed
//     std::void_t<...> -> void
//   * Specialization (2) matches <A, void>
// * Pick the most specialized version -> (2)

static_assert(!is_valid<B>::value, "is false");
// * Compare template arg list with primary template, we only supplied one
//   arg, the second one will be defaulted as
//   is_valid<A, void>
// * Compare template arg list against available specializations, this will
//   try to match the pattern <B, void> against the patterns defined in the
//   partial specializations.
// * Try specialization (2)
//   * T -> B
//   * Evaluate std::void_t -> decltype's are ill-formed
//   * Specialization (2) is removed from candidate set, no hard error (SFINAE)
// * No specialization matches, take the primary template.

int main() {}
