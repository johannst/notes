// Copyright (C) 2022 johannst

#include <iostream>

// Base case with one parameter.
template<int P>
void show_int() {
    printf("%d\n", P);
}

// General case with at least two parameters, to disambiguate from base case.
template<int P0, int P1, int... Params>
void show_int() {
    printf("%d, ", P0);
    show_int<P1, Params...>();
}

// Base case with one parameter.
template<typename T>
void show(const T& t) {
    std::cout << t << '\n';
}

// General case with at least two parameters, to disambiguate from base case.
template<typename T0, typename T1, typename... Types>
void show(const T0& t0, const T1& t1, const Types&... types) {
    std::cout << t0 << ", ";
    show(t1, types...);
}

int main() {
    show_int<1, 2, 3, 4, 5>();
    show(1, 1.0, "foo", 'a');
}
