// Copyright (C) 2023 johannst

#include <cstdio>
#include <utility>

struct M {};

// -- CONSUMER -----------------------------------------------------------------

void use(M&) {
    puts(__PRETTY_FUNCTION__);
}

void use(M&&) {
    puts(__PRETTY_FUNCTION__);
}

// -- TESTER -------------------------------------------------------------------

template<typename T>
void wrapper(T&& param) {  // forwarding reference
    puts(__PRETTY_FUNCTION__);
    // PARAM is an lvalue, therefore this always calls use(M&).
    use(param);
}

template<typename T>
void fwd_wrapper(T&& param) {  // forwarding reference
    puts(__PRETTY_FUNCTION__);
    // PARAM is an lvalue, but std::forward returns PARAM with the same value
    // category as the forwarding reference takes.
    use(std::forward<T>(param));
}

// -- MAIN ---------------------------------------------------------------------

int main() {
    {
        std::puts("==> wrapper rvalue reference");
        wrapper(M{});
        // calls use(M&).

        std::puts("==> wrapper lvalue reference");
        struct M m;
        wrapper(m);
        // calls use(M&).
    }
    {
        std::puts("==> fwd_wrapper rvalue reference");
        fwd_wrapper(M{});
        // calls use(M&&).

        std::puts("==> fwd_wrapper lvalue reference");
        struct M m;
        fwd_wrapper(m);
        // calls use(M&).
    }
}
