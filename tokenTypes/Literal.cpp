#include "Literal.h"

Literal::Literal(Type type, Value value)
        : type(type), value(std::move(value)) {}

Literal::Literal(const Value& val) {
    if (std::holds_alternative<std::string>(val)) {
        type = Type::TEXT;
        value = std::get<std::string>(val);
    } else if (std::holds_alternative<int>(val)) {
        type = Type::INTEGER;
        value = std::get<int>(val);
    } else if (std::holds_alternative<bool>(val)) {
        type = Type::BOOLEAN;
        value = std::get<bool>(val);
    } else {
        type = Type::NULL_VALUE;
        value = std::monostate{};
    }
}

std::string Literal::toString() const {
    switch (type) {
        case Type::TEXT:
            return "'" + std::get<std::string>(value) + "'";
        case Type::INTEGER:
            return std::to_string(std::get<int>(value));
        case Type::BOOLEAN:
            return std::get<bool>(value) ? "True" : "False";
        case Type::NULL_VALUE:
            return "NULL";
    }
    throw std::invalid_argument("Invalid literal Type enum value");
}

Literal::Value Literal::getValue() const {
    return value;
}

std::string Literal::typeToString(Literal::Type type) {
    switch (type) {
        case Type::TEXT:
            return "TEXT";
        case Type::INTEGER:
            return "INTEGER";
        case Type::BOOLEAN:
            return "BOOLEAN";
        case Type::NULL_VALUE:
            return "NULL";
    }
    throw std::invalid_argument("typeToString: Invalid literal type string");
}

Literal::Type Literal::getTypeFromValue(const std::string &tokenType) {
    if (tokenType == "TEXT") {
        return Type::TEXT;
    } else if (tokenType == "NUMBER") {
        return Type::INTEGER;
    } else if (tokenType == "BOOL") {
        return Type::BOOLEAN;
    } else if (tokenType == "NULL") {
        return Type::NULL_VALUE;
    } else {
        throw std::invalid_argument("Invalid literal type string: " + tokenType);
    }
}

Literal::Type Literal::getType() const {
    return type;
}
