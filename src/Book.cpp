#include "Book.h"
#include <utility>

Book::Book(std::string id, std::string title, std::string author,
           std::string category, int year, bool available)
    : id_(std::move(id)), title_(std::move(title)), author_(std::move(author)),
      category_(std::move(category)), year_(year), available_(available) {}

const std::string& Book::getId() const noexcept { return id_; }
const std::string& Book::getTitle() const noexcept { return title_; }
const std::string& Book::getAuthor() const noexcept { return author_; }
const std::string& Book::getCategory() const noexcept { return category_; }
int Book::getYear() const noexcept { return year_; }
bool Book::isAvailable() const noexcept { return available_; }

void Book::updateDetails(const std::string& title, const std::string& author,
                         const std::string& category, int year) {
    title_ = title; author_ = author; category_ = category; year_ = year;
}
void Book::setAvailable(bool available) noexcept { available_ = available; }
