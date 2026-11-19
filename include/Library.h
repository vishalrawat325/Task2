#pragma once
#include "Book.h"
#include "Member.h"
#include "Loan.h"
#include <string>
#include <vector>

class Library {
public:
    bool addBook(const Book& book, std::string& message);
    bool addMember(const Member& member, std::string& message);
    bool updateBook(const std::string& id, const std::string& title,
                    const std::string& author, const std::string& category,
                    int year, std::string& message);
    bool removeBook(const std::string& id, std::string& message);
    const Book* findBook(const std::string& id) const;
    std::vector<const Book*> searchBooks(const std::string& query) const;
    bool borrowBook(const std::string& bookId, const std::string& memberId,
                    const std::string& date, std::string& message);
    bool returnBook(const std::string& bookId, const std::string& date,
                    std::string& message);

    const std::vector<Book>& books() const noexcept;
    const std::vector<Member>& members() const noexcept;
    const std::vector<Loan>& loans() const noexcept;

    void replaceData(std::vector<Book> books, std::vector<Member> members,
                     std::vector<Loan> loans);

private:
    std::vector<Book> books_;
    std::vector<Member> members_;
    std::vector<Loan> loans_;
    static std::string lower(std::string value);
};
