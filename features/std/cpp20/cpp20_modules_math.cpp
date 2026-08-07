// description: Module interface for cpp20_modules_basic; exports a function without exposing textual implementation details.
// reference: https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Modules.html
// why: An importer needs a compiled interface rather than repeated textual inclusion.
// before: A header declared square and every consumer parsed that header again.
// pitfall: GCC modules require the interface to be compiled before its importers.

export module cpp20_modules_math;

export constexpr int square(int value) {
    return value * value;
}
