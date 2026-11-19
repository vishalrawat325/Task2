#pragma once
#include <string>

class Loan {
public:
    Loan() = default;
    Loan(std::string bookId, std::string memberId, std::string borrowDate);

    const std::string& getBookId() const noexcept;
    const std::string& getMemberId() const noexcept;
    const std::string& getBorrowDate() const noexcept;
    const std::string& getReturnDate() const noexcept;
    bool isActive() const noexcept;
    void close(const std::string& returnDate);

private:
    std::string bookId_;
    std::string memberId_;
    std::string borrowDate_;
    std::string returnDate_;
};
