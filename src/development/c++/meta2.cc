// Copyright (C) 2022 johannst

#include <iostream>
#include <type_traits>

// Helper meta fns.

template<typename T>
using enable_if_bool = std::enable_if_t<T::value, bool>;

template<typename T>
using disable_if_bool = std::enable_if_t<!T::value, bool>;

template<typename T>
using has_dst = std::integral_constant<bool, !std::is_same<typename T::Return, void>::value>;

// Template meta programming invoke machinery.

namespace impl {
    // Invoke an OPERATION which *USES* a context.
    template<typename Ctx, template<typename> class Op, typename... P,
             enable_if_bool<typename Op<Ctx>::HasCtx> = true>
    typename Op<Ctx>::Return Invoke(const Ctx& C, P... params) {
        return Op<Ctx>()(C, params...);
    }

    // Invoke an OPERATION which uses *NO* context.
    template<typename Ctx, template<typename> class Op, typename... P,
             disable_if_bool<typename Op<Ctx>::HasCtx> = true>
    typename Op<Ctx>::Return Invoke(const Ctx&, P... params) {
        return Op<Ctx>()(params...);
    }
}  // namespace impl

// Invoke an OPERATION which *HAS* a DESTINATION with arbitrary number of arguments.
template<typename Ctx, template<typename> class Op, typename... P,
         enable_if_bool<has_dst<Op<Ctx>>> = true>
void Invoke(const Ctx& C, P... params) {
    std::cout << "Invoke " << Op<Ctx>::Name << '\n';
    typename Op<Ctx>::Return R = impl::Invoke<Ctx, Op>(C, params...);
    std::cout << "returned -> " << R << '\n';
}

// Invoke an OPERATION which has *NOT* a DESTINATION with arbitrary number of arguments.
template<typename Ctx, template<typename> class Op, typename... P,
         disable_if_bool<has_dst<Op<Ctx>>> = true>
void Invoke(const Ctx& C, P... params) {
    std::cout << "Invoke " << Op<Ctx>::Name << " without destination." << '\n';
    impl::Invoke<Ctx, Op>(C, params...);
}

// Custom context.

struct Ctx {
    void out(const char* s, unsigned v) const { printf("%s%x\n", s, v); }
};

// Operations to invoke.

template<typename Ctx>
struct OpA {
    using HasCtx = std::false_type;
    using Return = int;
    static constexpr const char* const Name = "OpA";

    constexpr Return operator()(int a, int b) const { return a + b; }
};

template<typename Ctx>
struct OpB {
    using HasCtx = std::true_type;
    using Return = void;
    static constexpr const char* const Name = "OpB";

    Return operator()(const Ctx& C, unsigned a) const { C.out("a = ", a); }
};

int main() {
    Ctx C;

    Invoke<Ctx, OpA>(C, 1, 2);
    Invoke<Ctx, OpB>(C, 0xf00du);

    return 0;
}
