// Copyright (C) 2022 johannst

#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <type_traits>

template<typename R, typename... P>
struct registry {
    using FUNC = R (*)(P...);
    using SELF = registry<R, P...>;
    using RET = R;

    static SELF& get() {
        static SELF r;
        return r;
    }

    bool add(std::string nm, FUNC fn) {
        const auto r = m_fns.insert({std::move(nm), std::move(fn)});
        return r.second;
    }

    R invoke(const std::string& nm, P... p) const { return invoke_impl<R>(nm, p...); }

    void dump() const {
        for (const auto& it : m_fns) {
            std::puts(it.first.c_str());
        }
    }

  private:
    std::map<std::string, FUNC> m_fns;

    template<typename RET>
    std::enable_if_t<std::is_same_v<RET, void>> invoke_impl(const std::string& nm, P... p) const {
        const auto it = m_fns.find(nm);
        if (it == m_fns.end()) {
            return;
        }
        std::invoke(it->second, p...);
    }

    template<typename RET>
    std::enable_if_t<!std::is_same_v<RET, void>, RET> invoke_impl(const std::string& nm,
                                                                  P... p) const {
        const auto it = m_fns.find(nm);
        if (it == m_fns.end()) {
            static_assert(std::is_default_constructible_v<RET>,
                          "RET must be default constructible");
            return {};
        }
        return std::invoke(it->second, p...);
    }
};

#define TEST_REGISTER(REGISTRY, NAME)                                                      \
    static bool regfn_##REGISTRY##NAME() {                                                 \
        const bool r = REGISTRY::get().add(#NAME, NAME);                                   \
        if (!r) {                                                                          \
            std::puts("Failed to register test " #NAME ", same name already registered!"); \
            std::abort();                                                                  \
        }                                                                                  \
        return r;                                                                          \
    }                                                                                      \
    static const bool reg_##REGISTRY##NAME = regfn_##REGISTRY##NAME();

#define TEST(REGISTRY, NAME, ...)    \
    REGISTRY::RET NAME(__VA_ARGS__); \
    TEST_REGISTER(REGISTRY, NAME);   \
    REGISTRY::RET NAME(__VA_ARGS__)

// -- Usage 1 simple usage.

using REG1 = registry<void>;
TEST(REG1, test1) {
    std::puts("REG1::test1");
}
TEST(REG1, test2) {
    std::puts("REG1::test2");
}

// -- Usage 2 with convenience macro wrapper.

using REG2 = registry<void, bool>;
#define TEST2(NAME, ...) TEST(REG2, NAME, ##__VA_ARGS__)

TEST2(test1, bool val) {
    printf("REG2::test1 val %d\n", val);
}

int main() {
    const auto& R1 = REG1::get();
    R1.dump();
    R1.invoke("test1");
    R1.invoke("test2");

    const auto& R2 = REG2::get();
    R2.dump();
    R2.invoke("test1", true);

    return 0;
}
