#include "Loan.h"
#include <utility>

Loan::Loan(std::string bookId, std::string memberId, std::string borrowDate)
    : bookId_(std::move(bookId)), memberId_(std::move(memberId)),
      borrowDate_(std::move(borrowDate)) {}
const std::string& Loan::getBookId() const noexcept { return bookId_; }
const std::string& Loan::getMemberId() const noexcept { return memberId_; }
const std::string& Loan::getBorrowDate() const noexcept { return borrowDate_; }
const std::string& Loan::getReturnDate() const noexcept { return returnDate_; }
bool Loan::isActive() const noexcept { return returnDate_.empty(); }
void Loan::close(const std::string& returnDate) { returnDate_ = returnDate; }
