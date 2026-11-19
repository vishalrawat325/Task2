#pragma once
#include <string>

class Member {
public:
    Member() = default;
    Member(std::string id, std::string name, std::string email = {});

    const std::string& getId() const noexcept;
    const std::string& getName() const noexcept;
    const std::string& getEmail() const noexcept;

private:
    std::string id_;
    std::string name_;
    std::string email_;
};
