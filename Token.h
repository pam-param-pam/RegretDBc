#pragma once

#include <string>

class Token {
public:
    std::string type;
    std::string value;
    size_t position;

    Token(std::string  type, std::string  value, size_t position);

    [[nodiscard]] std::string toString() const;
};