#include "Literal.h"

Literal::Literal(Type type, Value value, std::optional<size_t> size)
        : type(type), value(std::move(value)), size(size) {}

std::string Literal::toString() const {
    switch (type) {
        case Type::TEXT:
            return std::get<std::string>(value);
        case Type::INTEGER:
            return std::to_string(std::get<int>(value));
        case Type::BOOLEAN:
            return std::get<bool>(value) ? "True" : "False";
    }
    throw std::invalid_argument("Invalid literal Type enum value");
}

Literal::Value Literal::getValue() {
    return value;
}
Literal::Type Literal::getType() {
    return type;
}
std::string Literal::typeToString(Literal::Type type) {
    switch (type) {
        case Type::TEXT: return "TEXT";
        case Type::INTEGER: return "INTEGER";
        case Type::BOOLEAN: return "BOOLEAN";
    }
    throw std::invalid_argument("typeToString: Invalid literal type string");
}
Literal::Type Literal::getTypeFromValue(const std::string& tokenType) {
    if (tokenType == "TEXT") {
        return Type::TEXT;
    } else if (tokenType == "NUMBER") {
        return Type::INTEGER;
    } else if (tokenType == "BOOL") {
        return Type::BOOLEAN;
    } else {
        throw std::invalid_argument("Invalid literal type string: " + tokenType);
    }
}