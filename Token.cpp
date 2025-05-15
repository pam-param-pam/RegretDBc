#include "Token.h"

Token::Token(std::string type, std::string value, int position)
        : type(std::move(type)), value(std::move(value)), position(position) {}

std::string Token::toString() const {
    return "Token(type: " + type + ", value: " + value + ", pos: " + std::to_string(position) + ")\n";
}
