#include "Literal.h"
#include "../exceptions/exceptions.h"

Literal::Literal(Type type, Value value)
        : type(type), value(std::move(value)) {}

Literal::Literal(const Value &val) {
    if (std::holds_alternative<std::string>(val)) {
        type = Type::TEXT;
        value = std::get<std::string>(val);
    } else if (std::holds_alternative<int>(val)) {
        type = Type::NUMBER;
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
            return std::get<std::string>(value);
        case Type::NUMBER:
            return std::to_string(std::get<int>(value));
        case Type::BOOLEAN:
            return std::get<bool>(value) ? "True" : "False";
        case Type::NULL_VALUE:
            return "NULL";
    }
    throw IntegrityError("Invalid literal Type enum value");
}

Literal::Value Literal::getValue() const {
    return value;
}

std::string Literal::typeToString(Literal::Type type) {
    switch (type) {
        case Type::TEXT:
            return "TEXT";
        case Type::NUMBER:
            return "NUMBER";
        case Type::BOOLEAN:
            return "BOOLEAN";
        case Type::NULL_VALUE:
            return "NULL";
    }
    throw IntegrityError("typeToString: Invalid literal type string");
}

Literal::Type Literal::getTypeFromValue(const std::string &tokenType) {
    if (tokenType == "TEXT") {
        return Type::TEXT;
    } else if (tokenType == "NUMBER") {
        return Type::NUMBER;
    } else if (tokenType == "BOOLEAN") {
        return Type::BOOLEAN;
    } else if (tokenType == "NULL") {
        return Type::NULL_VALUE;
    } else {
        throw IntegrityError("Invalid literal type string: " + tokenType);
    }
}

Literal::Type Literal::getType() const {
    return type;
}

bool Literal::isNull() const {
    return type == Type::NULL_VALUE;
}

bool Literal::operator==(const Literal &other) const {
    return type == other.type && value == other.value;
}

bool Literal::operator<(const Literal &other) const {
    if (isNull() && other.isNull()) return false;
    if (isNull()) return false;          // NULL is greater, so this is NOT less
    if (other.isNull()) return true;    // other NULL is greater, so this is less

    if (type != other.type) {
        return type < other.type;
    }

    return value < other.getValue();
}