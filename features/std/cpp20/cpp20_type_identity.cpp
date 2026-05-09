// description: C++20 added std::type_identity<T> (block template argument deduction in non-deduced contexts) and std::remove_cvref<T> (the common 'strip cv + reference, but not array-to-pointer decay' transformation).
// reference: https://en.cppreference.com/w/cpp/types/type_identity

#include "support/demo.hpp"
#include <type_traits>

// std::type_identity_t<T> is just T -- but using it in a function parameter
// puts T in a *non-deduced context*, so the caller must spell T explicitly.
// Useful when you want to drive deduction from one parameter only.
template <typename T>
T clamp_to(T lo, T hi, std::type_identity_t<T> v) {
    return v < lo ? lo : (hi < v ? hi : v);
}

// Without type_identity, this overload would have ambiguous deduction between
// the int (literal) and the long arg; type_identity_t says "deduce T from
// lo/hi only, then convert v".
static_assert(std::is_same_v<std::type_identity_t<int>, int>);

// std::remove_cvref<T> = remove_cv + remove_reference. Before C++20 the
// idiom was std::decay_t<T>, but decay also turns arrays into pointers and
// functions into function pointers, which is often *not* what you want.
static_assert(std::is_same_v<std::remove_cvref_t<const int&>, int>);
static_assert(std::is_same_v<std::remove_cvref_t<volatile int&&>, int>);

// Key difference vs decay: arrays are preserved.
static_assert(std::is_same_v<std::remove_cvref_t<const int (&)[4]>, int[4]>);
static_assert(std::is_same_v<std::decay_t<const int (&)[4]>, const int*>);

int main() {
    demo::title("C++20 type identity / remove_cvref");
    long v = clamp_to<long>(0, 10, 42);  // 42L truncated to 10L
    DEMO_ASSERT(v == 10);

    // Drive T from lo/hi; v is implicitly converted from int -> long.
    long w = clamp_to<long>(0, 10, 5);
    DEMO_ASSERT(w == 5);

    demo::text("remove_cvref<const int&>", "int");
    demo::text("remove_cvref<int(&)[4]>", "int[4] (decay would give int*)");
    return 0;
}
