#include "Library.h"
#include <algorithm>
#include <cctype>

std::string Library::lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

bool Library::addBook(const Book& book, std::string& message) {
    if (book.getId().empty() || book.getTitle().empty() ||
        book.getAuthor().empty() || book.getCategory().empty() ||
        book.getYear() < 0 || book.getYear() > 2100) {
        message = "Book details are invalid."; return false;
    }
    if (findBook(book.getId())) { message = "A book with that ID already exists."; return false; }
    books_.push_back(book); message = "Book added successfully."; return true;
}

bool Library::addMember(const Member& member, std::string& message) {
    if (member.getId().empty() || member.getName().empty()) {
        message = "Member ID and name are required."; return false;
    }
    auto it = std::find_if(members_.begin(), members_.end(),
        [&](const Member& m) { return m.getId() == member.getId(); });
    if (it != members_.end()) { message = "A member with that ID already exists."; return false; }
    members_.push_back(member); message = "Member added successfully."; return true;
}

bool Library::updateBook(const std::string& id, const std::string& title,
                         const std::string& author, const std::string& category,
                         int year, std::string& message) {
    auto it = std::find_if(books_.begin(), books_.end(),
        [&](const Book& b) { return b.getId() == id; });
    if (it == books_.end()) { message = "Book not found."; return false; }
    if (title.empty() || author.empty() || category.empty() || year < 0 || year > 2100) {
        message = "Updated details are invalid."; return false;
    }
    it->updateDetails(title, author, category, year);
    message = "Book updated successfully."; return true;
}

bool Library::removeBook(const std::string& id, std::string& message) {
    auto it = std::find_if(books_.begin(), books_.end(),
        [&](const Book& b) { return b.getId() == id; });
    if (it == books_.end()) { message = "Book not found."; return false; }
    if (!it->isAvailable()) { message = "Return this book before deleting it."; return false; }
    books_.erase(it); message = "Book deleted successfully."; return true;
}

const Book* Library::findBook(const std::string& id) const {
    auto it = std::find_if(books_.begin(), books_.end(),
        [&](const Book& b) { return b.getId() == id; });
    return it == books_.end() ? nullptr : &*it;
}

std::vector<const Book*> Library::searchBooks(const std::string& query) const {
    std::vector<const Book*> result;
    const auto q = lower(query);
    for (const auto& book : books_) {
        if (lower(book.getId()).find(q) != std::string::npos ||
            lower(book.getTitle()).find(q) != std::string::npos ||
            lower(book.getAuthor()).find(q) != std::string::npos ||
            lower(book.getCategory()).find(q) != std::string::npos) {
            result.push_back(&book);
        }
    }
    return result;
}

bool Library::borrowBook(const std::string& bookId, const std::string& memberId,
                         const std::string& date, std::string& message) {
    auto book = std::find_if(books_.begin(), books_.end(),
        [&](const Book& b) { return b.getId() == bookId; });
    if (book == books_.end()) { message = "Book not found."; return false; }
    if (!book->isAvailable()) { message = "Book is already borrowed."; return false; }
    auto member = std::find_if(members_.begin(), members_.end(),
        [&](const Member& m) { return m.getId() == memberId; });
    if (member == members_.end()) { message = "Member not found. Add the member first."; return false; }
    if (date.empty()) { message = "Borrow date is required."; return false; }
    book->setAvailable(false);
    loans_.emplace_back(bookId, memberId, date);
    message = "Book borrowed successfully."; return true;
}

bool Library::returnBook(const std::string& bookId, const std::string& date,
                         std::string& message) {
    auto book = std::find_if(books_.begin(), books_.end(),
        [&](const Book& b) { return b.getId() == bookId; });
    if (book == books_.end()) { message = "Book not found."; return false; }
    if (book->isAvailable()) { message = "This book is not currently borrowed."; return false; }
    auto loan = std::find_if(loans_.rbegin(), loans_.rend(),
        [&](const Loan& l) { return l.getBookId() == bookId && l.isActive(); });
    if (loan == loans_.rend()) { message = "Active loan record not found."; return false; }
    if (date.empty() || date < loan->getBorrowDate()) {
        message = "Return date is invalid or earlier than borrow date."; return false;
    }
    loan->close(date);
    book->setAvailable(true);
    message = "Book returned successfully."; return true;
}

const std::vector<Book>& Library::books() const noexcept { return books_; }
const std::vector<Member>& Library::members() const noexcept { return members_; }
const std::vector<Loan>& Library::loans() const noexcept { return loans_; }

void Library::replaceData(std::vector<Book> books, std::vector<Member> members,
                          std::vector<Loan> loans) {
    books_ = std::move(books); members_ = std::move(members); loans_ = std::move(loans);
}
