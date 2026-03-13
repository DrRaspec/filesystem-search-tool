#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void print_usage(const char *prog) {
    std::cerr << "Usage: " << prog << " [options] <filename> <directory>\n"
              << "\nOptions:\n"
              << "  -i, --ignore-case   Case-insensitive search\n"
              << "  -h, --help          Show this help message\n"
              << "\nExample:\n"
              << "  " << prog << " \"main.cpp\" \"C:\\Projects\"\n"
              << "  " << prog << " -i \"readme\" \".\"\n";
}

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    bool ignore_case = false;
    std::vector<std::string> positional;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-i" || arg == "--ignore-case") {
            ignore_case = true;
        } else {
            positional.push_back(arg);
        }
    }

    if (positional.size() < 2) {
        std::cerr
            << "Error: expected <filename> and <directory> arguments.\n\n";
        print_usage(argv[0]);
        return 1;
    }

    std::string target = positional[0];
    fs::path search_dir = positional[1];

    std::error_code ec;
    if (!fs::exists(search_dir, ec) || !fs::is_directory(search_dir, ec)) {
        std::cerr << "Error: \"" << search_dir.string()
                  << "\" is not a valid directory.\n";
        return 1;
    }

    std::string target_lower;
    if (ignore_case) {
        target_lower = to_lower(target);
    }

    int match_count = 0;

    auto options = fs::directory_options::skip_permission_denied;
    auto it = fs::recursive_directory_iterator(search_dir, options, ec);

    if (ec) {
        std::cerr << "Error: cannot read directory \"" << search_dir.string()
                  << "\": " << ec.message() << '\n';
        return 1;
    }

    for (auto &entry : it) {
        std::string filename = entry.path().filename().string();

        bool found = false;
        if (ignore_case) {
            found = to_lower(filename).find(target_lower) != std::string::npos;
        } else {
            found = filename.find(target) != std::string::npos;
        }

        if (found) {
            std::cout << "Found: " << entry.path().string() << '\n';
            ++match_count;
        }
    }

    std::cout << "\n"
              << match_count << " match" << (match_count != 1 ? "es" : "")
              << " found.\n";

    return 0;
}