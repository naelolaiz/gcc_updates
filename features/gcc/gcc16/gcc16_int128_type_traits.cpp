// description: GCC 16 makes standard type traits recognize __int128 even in strict -std=c++NN modes, aligning strict and GNU dialect behavior.
// reference: https://gcc.gnu.org/gcc-16/changes.html
// why: Generic numeric code can now classify GCC's 128-bit integers consistently in strict mode.
// before: Code special-cased __int128 or selected a gnu++ dialect solely for trait support.
// pitfall: __int128 remains a compiler extension, despite working with these standard traits.

#include "support/demo.hpp"
#include <type_traits>

__extension__ using int128_t = __int128;
__extension__ using uint128_t = unsigned __int128;

int main() {
    demo::title("GCC 16 int128 type traits");
    static_assert(std::is_integral_v<int128_t>);
    static_assert(std::is_integral_v<uint128_t>);
    static_assert(std::is_signed_v<int128_t>);
    static_assert(std::is_unsigned_v<uint128_t>);
    demo::text("check", "strict-mode type traits recognize 128-bit integers");
    return 0;
}
