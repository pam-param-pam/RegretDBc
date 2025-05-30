#include "Identifier.h"
#include "../exceptions/exceptions.h"

Identifier::Identifier(Type type, std::string value)
        : type(type), value(std::move(value)) {}

std::string Identifier::toString() const {
    return typeToString(type) + "(" + value + ")";
}

std::string Identifier::typeToString(Type type) {
    switch (type) {
        case Type::COLUMN:
            return "COLUMN";
        case Type::TABLE:
            return "TABLE";
        default:
            throw IntegrityError("Invalid identifier Type enum value");
    }
}
