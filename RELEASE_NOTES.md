## v1.1.0 - 2026-03-13

### Added
- Search duration output at the end of each run (`Search time: <ms> ms`).

### Changed
- Improved traversal performance with iterative directory scanning.
- Optimized case-insensitive filename matching to reduce per-entry overhead.

### Fixed
- Resolved full-drive scan issue where traversal could stop early on problematic folders.
- Improved resilience by skipping unreadable directories and continuing the search.

---

### 🎉 Initial Release: Filesystem Search Tool v1.0.0

A fast, lightweight command-line tool written in C++ for searching files and directories across your system.

#### ✨ Features
* **Recursive searching**: Easily find files nested deep within directories.
* **Partial matching**: Searches for substrings within filenames.
* **Case-insensitive search**: Use the `-i` or `--ignore-case` flag to ignore capitalization.
* **Safe traversal**: Automatically skips permission-denied directories without crashing.

#### 🚀 Quick Start
Download the attached `.exe` file below and run it from your command prompt or terminal:

```bash
# Basic usage
search.exe [options] <filename> <directory>

# Example: Search for "readme" inside the current directory
search.exe "readme" "."

# Example: Case-insensitive search for "main.cpp" in C:\Projects
search.exe -i "main.cpp" "C:\Projects"
```

**Full Changelog**: https://github.com/DrRaspec/filesystem-search-tool/commits/v1.0.0
