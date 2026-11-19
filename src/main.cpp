#include "Library.h"
#include "Storage.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

namespace {
const std::string DATA_FILE = "data/library.db";

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}
int readInt(const std::string& prompt, int min, int max) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            std::size_t used = 0;
            int value = std::stoi(s, &used);
            if (used == s.size() && value >= min && value <= max) return value;
        } catch (...) {}
        std::cout << "Invalid input. Enter a number from " << min << " to " << max << ".\n";
    }
}
void printBook(const Book& b) {
    std::cout << std::left << std::setw(10) << b.getId()
              << std::setw(27) << b.getTitle().substr(0, 25)
              << std::setw(22) << b.getAuthor().substr(0, 20)
              << std::setw(16) << b.getCategory()
              << std::setw(8) << b.getYear()
              << (b.isAvailable() ? "Available" : "Borrowed") << '\n';
}
void printMenu() {
    std::cout << "\n========== OBJECT-ORIENTED LIBRARY MANAGER ==========\n"
              << "1. Add book\n2. List books\n3. Search books\n4. Update book\n"
              << "5. Delete book\n6. Add member\n7. Borrow book\n8. Return book\n"
              << "9. Library statistics\n10. Save now\n0. Save and exit\n"
              << "=====================================================\n";
}
}

int main() {
    Library library;
    std::string message;
    Storage::load(library, DATA_FILE, message);
    std::cout << message << "\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Choose an option: ", 0, 10);
        std::string id, title, author, category;
        int year;
        bool changed = false;

        switch (choice) {
        case 1: {
            id = readLine("Book ID: ");
            title = readLine("Title: ");
            author = readLine("Author: ");
            category = readLine("Category: ");
            year = readInt("Publication year (0-2100): ", 0, 2100);
            changed = library.addBook(Book(id, title, author, category, year), message);
            std::cout << message << '\n'; break;
        }
        case 2:
            if (library.books().empty()) std::cout << "No books in the library yet.\n";
            else {
                std::cout << std::left << std::setw(10) << "ID" << std::setw(27) << "TITLE"
                          << std::setw(22) << "AUTHOR" << std::setw(16) << "CATEGORY"
                          << std::setw(8) << "YEAR" << "STATUS\n";
                for (const auto& b : library.books()) printBook(b);
            } break;
        case 3: {
            auto q = readLine("Search ID/title/author/category: ");
            auto results = library.searchBooks(q);
            if (results.empty()) std::cout << "No matching books found.\n";
            else for (const auto* b : results) printBook(*b);
            break;
        }
        case 4: {
            id = readLine("Book ID to update: ");
            title = readLine("New title: "); author = readLine("New author: ");
            category = readLine("New category: ");
            year = readInt("New publication year (0-2100): ", 0, 2100);
            changed = library.updateBook(id, title, author, category, year, message);
            std::cout << message << '\n'; break;
        }
        case 5:
            id = readLine("Book ID to delete: ");
            changed = library.removeBook(id, message);
            std::cout << message << '\n'; break;
        case 6: {
            id = readLine("Member ID: ");
            auto name = readLine("Member name: ");
            auto email = readLine("Member email (optional): ");
            changed = library.addMember(Member(id, name, email), message);
            std::cout << message << '\n'; break;
        }
        case 7: {
            id = readLine("Book ID: ");
            auto member = readLine("Member ID: ");
            auto date = readLine("Borrow date (YYYY-MM-DD): ");
            changed = library.borrowBook(id, member, date, message);
            std::cout << message << '\n'; break;
        }
        case 8: {
            id = readLine("Book ID: ");
            auto date = readLine("Return date (YYYY-MM-DD): ");
            changed = library.returnBook(id, date, message);
            std::cout << message << '\n'; break;
        }
        case 9: {
            std::size_t borrowed = 0;
            for (const auto& b : library.books()) if (!b.isAvailable()) ++borrowed;
            std::cout << "Total books: " << library.books().size()
                      << "\nAvailable: " << library.books().size() - borrowed
                      << "\nBorrowed: " << borrowed
                      << "\nMembers: " << library.members().size()
                      << "\nLoan records: " << library.loans().size() << '\n';
            break;
        }
        case 10:
            if (Storage::save(library, DATA_FILE, message)) std::cout << message << '\n';
            else std::cerr << message << '\n';
            break;
        case 0:
            if (Storage::save(library, DATA_FILE, message)) std::cout << message << '\n';
            else std::cerr << message << '\n';
            running = false; break;
        }
        if (changed && !Storage::save(library, DATA_FILE, message))
            std::cerr << "Warning: " << message << '\n';
    }
    std::cout << "Goodbye!\n";
    return 0;
}
