// Copyright (C) 2023 johannst

enum Kind {
    kPrimary,
    kTT,
    kIntBool,
    kIntInt,
};

// (1) Primary template.
template<typename T, typename U = bool>
struct pair {
    static constexpr Kind kind = kPrimary;
};

// (2) Partial template specialization.
template<typename T>
struct pair<T, T> {
    static constexpr Kind kind = kTT;
};

// (3) Template specialization.
template<>
struct pair<int, bool> {
    static constexpr Kind kind = kIntBool;
};

// (4) Template specialization.
template<>
struct pair<int, int> {
    static constexpr Kind kind = kIntInt;
};

int main() {
    static_assert(pair<int>::kind == kIntBool, "");
    // * Compare template arg list with primary template, we only supplied one
    //   arg, the second one will be defaulted as
    //   pair<int, bool>
    // * Compare template arg list against available specializations, this will
    //   try to match the pattern <int, bool> against the patterns defined in the
    //   partial specializations.
    // * (2) <int, bool> pattern does not match
    // * (3) <int, bool> pattern does match
    // * (4) <int, bool> pattern does not match
    // * Pick the most specialized version -> (3)

    static_assert(pair<char, char>::kind == kTT, "");
    // * Compare template arg list against available specializations, this will
    //   try to match the pattern <char, char> against the patterns defined in the
    //   partial specializations.
    // * (2) <char, char> pattern does match
    // * (3) <char, char> pattern does not match
    // * (4) <char, char> pattern does not match
    // * Pick the most specialized version -> (2)

    static_assert(pair<int, int>::kind == kIntInt, "");
    // * Compare template arg list against available specializations, this will
    //   try to match the pattern <int, int> against the patterns defined in the
    //   partial specializations.
    // * (2) <int, int> pattern does match
    // * (3) <int, int> pattern does match
    // * (4) <int, int> pattern does not match
    // * Pick the most specialized version -> (3)

    static_assert(pair<char, short>::kind == kPrimary, "");
    // * Compare template arg list against available specializations, this will
    //   try to match the pattern <char, short> against the patterns defined in the
    //   partial specializations.
    // * (2) <char, short> pattern does not match
    // * (3) <char, short> pattern does not match
    // * (4) <char, short> pattern does not match
    // * No specialization matches, take the primary template.
}
