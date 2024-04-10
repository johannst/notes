#include <concepts>
#include <type_traits>

// REQUIRES EXPRESSION
//   requires { requirement-seq }
//   requires ( parameter-list ) { requirement-seq }
//
// [1] https://en.cppreference.com/w/cpp/language/requires
// [2] https://en.cppreference.com/w/cpp/language/constraints#Constraints
//
// REQUIREMENT CLAUSE
//   Not the same as a REQUIREMENT EXPRESSIONS, and is used to require
//   constraints (express concept bounds).
//
// [1] https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses

// -- HELPER -------------------------------------------------------------------

template<typename T>
using Alias = T;

void print(int);

// -- CONCEPTS & REQUIRE EXPRESSIONS -------------------------------------------

// Simple concept from a type trait.
template<typename T, typename U>
concept Same = std::is_same<T, U>::value;

// Simple requirement concepts.
template<typename T>
concept TraitAddAndPrint = requires(T t, int i) {
    // Adding T + int must be supported.
    t + i;
    // Calling print(T) must be available.
    print(t);
};

// Type requirement concepts.
template<typename T>
concept TraitTypes = requires(T t) {
    // T must have a type definition inner.
    typename T::inner;
    // Type alias must exist.
    typename Alias<T>;
};

// Compound requirement concepts.
template<typename T>
concept TraitFns = requires(T t, const T c) {
    // void T::foo() must exist.
    { t.foo() };
    // bool T::bar() const; must exist.
    { c.bar() } -> Same<bool>;
    // static void T::stat(); must exist.
    { T::stat() } -> Same<int>;
};

// Nested requirement concepts.
template<typename T>
concept TraitNested = requires(T t) {
    // Must satisfy other concepts.
    requires TraitTypes<T>;
    requires TraitFns<T>;
};

// -- REQUIRE EXPRESSIONS ------------------------------------------------------

// Require expressions can be evaluated to booleans.
template<typename T>
static constexpr bool IsTraitFns = requires { requires TraitFns<T>; };

// Require expressions can also be used in static assertions.
static_assert(requires { requires Same<int, int>; });
static_assert(!requires {
    typename Alias<int>;
    requires Same<int, void>;
});

// -- TESTS --------------------------------------------------------------------

static_assert(requires { requires TraitAddAndPrint<int>; });

struct FnTypeGood {
    using inner = int;
};
struct FnTypeBad {};
static_assert(requires { requires TraitTypes<FnTypeGood>; });
static_assert(!requires { requires TraitTypes<FnTypeBad>; });

struct FnGood {
    void foo();
    bool bar() const;
    static int stat();
};
struct FnBad {};
static_assert(requires { requires TraitFns<FnGood>; });
static_assert(!requires { requires TraitFns<FnBad>; });

struct NestedGood : FnTypeGood, FnGood {};
struct NestedBad1 : FnGood {};
struct NestedBad2 : FnTypeGood {};
static_assert(requires { requires TraitNested<NestedGood>; });
static_assert(!requires { requires TraitNested<NestedBad1>; });
static_assert(!requires { requires TraitNested<NestedBad2>; });
