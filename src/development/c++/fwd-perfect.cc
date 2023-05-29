// Copyright (C) 2023 johannst

#include <cassert>
#include <cstdio>
#include <new>
#include <type_traits>
#include <utility>

struct S {};

struct M {
    M() {
        std::puts("M()");
    }
    M(const M&) {
        std::puts("M(M&)");
    }
    M(M&&) {
        std::puts("M(M&&)");
    }
    M& operator=(const M&) = delete;
    M& operator=(M&&) = delete;

    M(S&, int) {
        std::puts("M(S&)");
    }
    M(S&&, int) {
        std::puts("M(S&&)");
    }
    ~M() {
        std::puts("~M()");
    }
};

template<typename T>
struct option {
    static_assert(!std::is_reference_v<T>);

    constexpr option() = default;

    template<typename... Params>
    constexpr option(Params&&... params) : m_has_val(true) {
        // BAD: does not perfectly forward!
        //      eg, if option(S&&) is invoked, this would invoke M(S&).
        // new (&m_val) T(params...);

        // GOOD: perfectly forwards params to constructor of T.
        new (m_val) T(std::forward<Params>(params)...);
    }

    ~option() {
        reset();
    }

    constexpr T& value() {
        assert(m_has_val);
        return *reinterpret_cast<T*>(m_val);
    }

  private:
    constexpr void reset() {
        if (!m_has_val) {
            return;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            value().~T();
        };
    }

    alignas(T) char m_val[sizeof(T)];
    bool m_has_val{false};
};

int main() {
    std::puts("==> case 1");
    // invokes M(S&&, int)
    option<M> opt1(S{}, 123);

    std::puts("==> case 2");
    // invokes M() + M(M&&)
    option<M> x /* option(M&&) + M(M&&) */ = M{} /* M() */;
}
