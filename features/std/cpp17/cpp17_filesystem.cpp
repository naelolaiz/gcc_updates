// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: <filesystem> standardised path manipulation, directory iteration, and basic file ops.
// reference: https://en.cppreference.com/w/cpp/filesystem

#include "support/demo.hpp"
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main() {
    demo::title("C++17 filesystem");
    // Path manipulation -- all in-memory, no IO.
    fs::path p = "/tmp/some/dir/file.txt";
    DEMO_ASSERT(p.parent_path() == "/tmp/some/dir");
    DEMO_ASSERT(p.filename() == "file.txt");
    DEMO_ASSERT(p.stem() == "file");
    DEMO_ASSERT(p.extension() == ".txt");

    fs::path joined = fs::path("/etc") / "hosts";
    DEMO_ASSERT(joined == "/etc/hosts");

    // Light filesystem IO in a unique temp dir.
    auto tmp = fs::temp_directory_path() / "gcc_updates_cpp17_filesystem";
    fs::remove_all(tmp);
    fs::create_directories(tmp);
    DEMO_ASSERT(fs::is_directory(tmp));

    fs::path f = tmp / "hello.txt";
    {
        std::ofstream out(f);
        out << "hi";
    }
    DEMO_ASSERT(fs::exists(f));
    DEMO_ASSERT(fs::file_size(f) == 2);

    int count = 0;
    for ([[maybe_unused]] auto& e : fs::directory_iterator(tmp)) ++count;
    DEMO_ASSERT(count == 1);

    fs::remove_all(tmp);
    DEMO_ASSERT(!fs::exists(tmp));
    return 0;
}
