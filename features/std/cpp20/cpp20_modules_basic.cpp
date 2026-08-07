// description: C++20 modules compile an interface before an importer; this two-file fixture proves export, import, link, and execution with GCC's modules implementation.
// reference: https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Modules.html
// why: Modules replace repeated textual inclusion with an explicit compiled interface.
// before: A header declaration plus a separately compiled implementation file.
// pitfall: GCC 16 module support remains experimental and requires -fmodules.

import cpp20_modules_math;

int main() {
    return square(6) == 36 ? 0 : 1;
}
