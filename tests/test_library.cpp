#include "Library.h"
#include <iostream>
#include <string>

namespace {
int failures = 0;
void check(bool condition, const std::string& name) {
    if (condition) std::cout << "[PASS] " << name << '\n';
    else { std::cout << "[FAIL] " << name << '\n'; ++failures; }
}
}

int main() {
    Library library;
    std::string message;

    check(library.books().empty(), "starts with an empty collection");
    check(library.searchBooks("missing").empty(), "empty search returns no results");

    check(library.addBook(Book("B1", "Clean Code", "Robert Martin", "Technology", 2008), message),
          "adds a valid book");
    check(!library.addBook(Book("B1", "Duplicate", "Someone", "Other", 2020), message),
          "rejects duplicate book IDs");
    check(library.searchBooks("clean").size() == 1, "case-insensitive title search");

    check(library.updateBook("B1", "Clean Code 2", "Robert Martin", "Technology", 2010, message),
          "updates an existing book");
    check(!library.updateBook("NOPE", "X", "Y", "Z", 2020, message),
          "rejects update for missing ID");

    check(library.addMember(Member("M1", "Asha"), message), "adds a member");
    check(library.borrowBook("B1", "M1", "2026-01-10", message), "borrows an available book");
    check(!library.borrowBook("B1", "M1", "2026-01-11", message),
          "rejects borrowing an unavailable book");
    check(!library.removeBook("B1", message), "prevents deleting a borrowed book");
    check(!library.returnBook("B1", "2026-01-09", message),
          "rejects return date before borrow date");
    check(library.returnBook("B1", "2026-01-12", message), "returns a borrowed book");
    check(!library.returnBook("B1", "2026-01-13", message),
          "rejects returning a book that is already available");

    std::cout << "\nTest result: " << (failures == 0 ? "ALL PASSED" : "FAILURES FOUND")
              << " (" << failures << " failed)\n";
    return failures == 0 ? 0 : 1;
}
