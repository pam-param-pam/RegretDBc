#pragma once

#include <string>

class Token {
public:
    std::string type;
    std::string value;
    int position;

    Token(std::string type, std::string value, int position);

    [[nodiscard]] std::string toString() const;
};