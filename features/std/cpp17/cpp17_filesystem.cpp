// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: <filesystem> standardised path manipulation, directory iteration, and basic file ops.
// reference: https://en.cppreference.com/w/cpp/filesystem

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main() {
    // Path manipulation -- all in-memory, no IO.
    fs::path p = "/tmp/some/dir/file.txt";
    assert(p.parent_path() == "/tmp/some/dir");
    assert(p.filename() == "file.txt");
    assert(p.stem() == "file");
    assert(p.extension() == ".txt");

    fs::path joined = fs::path("/etc") / "hosts";
    assert(joined == "/etc/hosts");

    // Light filesystem IO in a unique temp dir.
    auto tmp = fs::temp_directory_path() / "gcc_updates_cpp17_filesystem";
    fs::remove_all(tmp);
    fs::create_directories(tmp);
    assert(fs::is_directory(tmp));

    fs::path f = tmp / "hello.txt";
    {
        std::ofstream out(f);
        out << "hi";
    }
    assert(fs::exists(f));
    assert(fs::file_size(f) == 2);

    int count = 0;
    for ([[maybe_unused]] auto& e : fs::directory_iterator(tmp)) ++count;
    assert(count == 1);

    fs::remove_all(tmp);
    assert(!fs::exists(tmp));
    return 0;
}
