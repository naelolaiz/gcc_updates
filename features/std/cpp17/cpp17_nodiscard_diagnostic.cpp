// description: [[nodiscard]] turns an ignored result into a diagnostic; -Werror makes the negative example an executable CI claim.
// reference: https://en.cppreference.com/w/cpp/language/attributes/nodiscard
// why: Ignoring an error-bearing or resource-bearing result is often a caller bug.
// before: APIs relied on documentation and code review to make callers inspect results.
// pitfall: The attribute diagnoses discarding; it does not prescribe how the value is handled.

[[nodiscard("check the parse result")]] int parse() {
    return 42;
}

int main() {
    parse();
}
