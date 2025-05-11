// Token.cpp
#include "Token.h"

Token::Token(std::string  type, std::string  value, size_t position)
        : type(std::move(type)), value(std::move(value)), position(position) {}
// Define a to_string method
std::string Token::toString() const {
    return "Token(type: " + type + ", value: " + value + ", pos: " + std::to_string(position) + ")\n";
}