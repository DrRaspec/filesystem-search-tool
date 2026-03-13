# File Search Tool

A small C++ command-line utility for searching filenames inside a directory tree.

This project was built as a simple, fast way to scan a folder recursively and print every file whose name matches a search term. It supports both normal matching and case-insensitive matching.

## What it does

- Recursively walks through a directory
- Looks for filenames that contain the text you pass in
- Prints the full path for each match
- Skips permission-denied folders instead of crashing
- Supports case-insensitive search with `-i`

## Build

This project is set up to build with `g++` using C++17.

```powershell
g++ -g -std=c++17 -o search.exe search.cpp
```

If you're using the included VS Code config, you can also build from the editor with the default build task.

## Usage

```powershell
search.exe [options] <filename> <directory>
```

Examples:

```powershell
search.exe "main.cpp" "C:\Projects"
search.exe -i "readme" "."
```

Options:

- `-i`, `--ignore-case`  
  Search without caring about uppercase or lowercase letters

- `-h`, `--help`  
  Show the help message

## Example output

```text
Found: C:\Projects\App\main.cpp
Found: C:\Projects\Tools\main.cpp

2 matches found.
```

## Notes on code style

This code uses `std::string`, `std::cout`, and similar names explicitly instead of `using namespace std;`.

That is intentional.

Using `std::` keeps it clear where names come from and avoids namespace conflicts as the code grows. `using namespace std;` is common in quick examples, but in real projects it tends to create avoidable problems, especially once multiple headers and libraries are involved.

The project also uses:

- `namespace fs = std::filesystem;`

That alias keeps filesystem code shorter without pulling the entire standard library into the global namespace.

## Requirements

- C++17 or newer
- A compiler with `<filesystem>` support

## Future improvements

- Filter by file extension
- Match full file names exactly
- Add optional output to a text file
- Add support for regex search
