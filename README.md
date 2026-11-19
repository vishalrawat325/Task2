# Object-Oriented Library Manager

A menu-driven, offline C++17 library manager demonstrating classes, encapsulation, STL collections, and local file persistence.

## Features

- Add, list, search, update, and delete book records.
- Register library members.
- Borrow and return books with loan history.
- Prevent duplicate book/member IDs and borrowing unavailable books.
- Search by ID, title, author, or category (case-insensitive).
- Save changes locally and reload on startup.
- Load sample data from `data/library.db`.
- Skip malformed storage rows instead of terminating.

## Architecture

| Component | Responsibility |
|---|---|
| `Book` | Encapsulates book identity, metadata, and availability |
| `Member` | Encapsulates member identity and contact details |
| `Loan` | Records borrower, book, borrow date, and return date |
| `Library` | Owns collections and enforces library business rules |
| `Storage` | Serializes and restores library records |
| `main.cpp` | Handles terminal input/output and menu navigation |

## Ownership and RAII

`Library` owns its `Book`, `Member`, and `Loan` objects by value in `std::vector` containers. These objects own their `std::string` members by value. There are no raw owning pointers, manual `new`/`delete` calls, or long-lived file handles. `std::ifstream` and `std::ofstream` manage file resources through RAII. Search results return temporary non-owning `const Book*` pointers into the library's vector; callers must not retain them across mutations that may reallocate the vector.

## Build

Requires a C++17 compiler. CMake is optional if you use the direct compiler command.

### CMake

```bash
cmake -S . -B build
cmake --build build
```

Run on Windows:

```powershell
.\build\Debug\library_manager.exe
```

Depending on the generator, the executable may instead be at `build\library_manager.exe`.

Run on Linux/macOS:

```bash
./build/library_manager
```

### Direct GCC build (no CMake required)

From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Library.cpp src/Storage.cpp src/Book.cpp src/Member.cpp src/Loan.cpp -o library_manager
```

Run on Windows:

```powershell
.\library_manager.exe
```

Run on Linux/macOS:

```bash
./library_manager
```

## Data format

The application stores records in `data/library.db` using three labelled sections: `[BOOKS]`, `[MEMBERS]`, and `[LOANS]`. Fields are quoted and quotes inside values are doubled. The included file contains fictional sample records.

## Complexity notes

Let `B` be the number of books, `M` members, and `L` loan records.

- Add book: `O(B)` because duplicate ID validation scans the vector.
- Find book by ID: `O(B)`.
- Search by text: `O(B)` comparisons, plus string matching cost.
- List books: `O(B)`.
- Borrow: `O(B + M + L)` worst case with current vector-based lookup.
- Return: `O(B + L)`.
- Save/load: `O(B + M + L)` relative to record count and serialized data size.

A future optimization could add `unordered_map` indexes for average `O(1)` ID lookup while retaining vectors for predictable iteration.

## Limitations

- The CLI expects dates in `YYYY-MM-DD` format. The current implementation checks ordering lexicographically but does not fully validate calendar dates.
- Storage replacement uses a temporary file; replacement behavior can vary by operating system.
- This is a local single-user application, not a networked or multi-user service.

## Suggested next improvements

Add full calendar-date validation, a unit-test target, CSV import/export, and a stronger cross-platform atomic-save strategy.


## Automated tests

Build the test target and run:

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

The test executable covers empty collections, duplicate IDs, search, update, missing IDs,
member registration, borrow/return rules, and date ordering.

See [CLASS_DIAGRAM.md](CLASS_DIAGRAM.md) for the class relationship diagram.
