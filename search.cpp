#include <algorithm>
#include <cctype>
#include <chrono>
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

bool should_skip_directory(const fs::path &path) {
    const std::string name = path.filename().string();
    return name == "$RECYCLE.BIN" || name == "System Volume Information";
}

bool contains_case_insensitive(const std::string &text,
                               const std::string &needle_lower) {
    if (needle_lower.empty()) {
        return true;
    }
    if (needle_lower.size() > text.size()) {
        return false;
    }

    const size_t last = text.size() - needle_lower.size();
    for (size_t i = 0; i <= last; ++i) {
        size_t j = 0;
        for (; j < needle_lower.size(); ++j) {
            const unsigned char c = static_cast<unsigned char>(text[i + j]);
            if (static_cast<char>(std::tolower(c)) != needle_lower[j]) {
                break;
            }
        }
        if (j == needle_lower.size()) {
            return true;
        }
    }

    return false;
}

void scan(const fs::path &root, const std::string &target,
          const std::string &target_lower, bool ignore_case, int &match_count) {
    std::vector<fs::path> pending_dirs;
    pending_dirs.push_back(root);

    while (!pending_dirs.empty()) {
        const fs::path current_dir = std::move(pending_dirs.back());
        pending_dirs.pop_back();

        std::error_code ec;
        fs::directory_iterator it(
            current_dir, fs::directory_options::skip_permission_denied, ec);
        if (ec) {
            continue;
        }

        const fs::directory_iterator end;
        while (it != end) {
            const fs::directory_entry &entry = *it;
            const fs::path entry_path = entry.path();

            const std::string filename = entry_path.filename().string();
            bool found = false;

            if (ignore_case) {
                found = contains_case_insensitive(filename, target_lower);
            } else {
                found = filename.find(target) != std::string::npos;
            }

            if (found) {
                std::cout << "Found: " << entry_path.string() << '\n';
                ++match_count;
            }

            std::error_code is_dir_ec;
            if (entry.is_directory(is_dir_ec) && !is_dir_ec &&
                !should_skip_directory(entry_path)) {
                pending_dirs.push_back(entry_path);
            }

            it.increment(ec);
            if (ec) {
                ec.clear();
            }
        }
    }
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
    const auto start_time = std::chrono::steady_clock::now();
    scan(search_dir, target, target_lower, ignore_case, match_count);
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start_time)
            .count();

    std::cout << "\n"
              << match_count << " match" << (match_count != 1 ? "es" : "")
              << " found.\n";
    std::cout << "Search time: " << elapsed_ms << " ms\n";

    return 0;
}