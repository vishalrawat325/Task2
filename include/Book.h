#pragma once
#include <string>

class Book {
public:
    Book() = default;
    Book(std::string id, std::string title, std::string author,
         std::string category, int year, bool available = true);

    const std::string& getId() const noexcept;
    const std::string& getTitle() const noexcept;
    const std::string& getAuthor() const noexcept;
    const std::string& getCategory() const noexcept;
    int getYear() const noexcept;
    bool isAvailable() const noexcept;

    void updateDetails(const std::string& title, const std::string& author,
                       const std::string& category, int year);
    void setAvailable(bool available) noexcept;

private:
    std::string id_;
    std::string title_;
    std::string author_;
    std::string category_;
    int year_ = 0;
    bool available_ = true;
};
