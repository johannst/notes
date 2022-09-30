// Copyright (C) 2022 johannst
#include <type_traits>

template<typename T, template<typename> class Checker, typename = void>
struct is_valid : std::false_type {};

template<typename T, template<typename> class Checker>
struct is_valid<T, Checker, std::void_t<Checker<T>>> : std::true_type {};

template<typename T, template<typename> class Checker>
static constexpr bool is_valid_v = is_valid<T, Checker>::value;

// -----------------------------------------------------------------------------

template<typename T, typename R, template<typename> class Checker, typename = void>
struct is_valid_with_ret : std::false_type {};

template<typename T, typename R, template<typename> class Checker>
struct is_valid_with_ret<T, R, Checker, std::void_t<Checker<T>>> : std::is_same<R, Checker<T>> {};

template<typename T, typename R, template<typename> class Checker>
static constexpr bool is_valid_with_ret_v = is_valid_with_ret<T, R, Checker>::value;

// -----------------------------------------------------------------------------

template<typename T>
struct is_entry {
    template<typename TT>
    using init = decltype(std::declval<TT>().init());
    template<typename TT>
    using tag = decltype(std::declval<TT>().tag());
    template<typename TT>
    using val = decltype(std::declval<TT>().val());

    static constexpr bool value = is_valid_v<T, init> &&
                                  is_valid_with_ret_v<T, int, tag> &&
                                  is_valid_with_ret_v<T, typename T::Type, val>;
};

template<typename T>
static constexpr bool is_entry_v = is_entry<T>::value;

template<typename E>
struct Entry {
    using Type = E;
    void init();
    int tag() const;
    E val() const;
};

int main() {
    static_assert(is_entry_v<Entry<bool>>, "");
}
