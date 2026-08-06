// description: Brace initialization rejects narrowing conversions at compile time; this negative example proves the diagnostic instead of leaving invalid code commented out.
// reference: https://en.cppreference.com/w/cpp/language/list_initialization
// why: Accidental precision loss should be rejected where a value is initialized.
// before: Parenthesized or assignment initialization silently converted the double to int.
// pitfall: Whether a conversion narrows can depend on whether a constant value is representable.

int main() {
    int count{1.5};
    return count;
}
