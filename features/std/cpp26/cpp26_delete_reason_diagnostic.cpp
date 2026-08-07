// description: Calling a function deleted with a reason must fail and surface the author-provided migration guidance in the compiler diagnostic.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2573r2.html
// why: A rejected legacy overload can tell users exactly which safe replacement to choose.
// before: A plain = delete diagnostic exposed the forbidden signature but not the migration reason.
// pitfall: Overload resolution still selects the best candidate before reporting that it is deleted.

#include <string_view>

void parse_config(const char*)
    = delete("use parse_config(std::string_view) so the length is tracked");
bool parse_config(std::string_view text) {
    return !text.empty();
}

int main() {
    parse_config("key=value");
}
