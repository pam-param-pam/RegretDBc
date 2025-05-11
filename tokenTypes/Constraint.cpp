#include "Constraint.h"


Constraint::Constraint(Type type, std::optional<std::string> arg1, std::optional<std::string> arg2) {
    this->type = type;
    this->arg1 = std::move(arg1);
    this->arg2 = std::move(arg2);
}

std::string Constraint::toString() const {
    std::string result = "Type: " + typeToString(type);
    if (arg1.has_value()) {
        result += " Arg1: " + arg1.value();
    }
    if (arg2.has_value()) {
        result += " Arg2: " + arg2.value();
    }
    return result;
}

std::string Constraint::typeToString(Type type) {
    switch (type) {
        case Type::PRIMARY_KEY:
            return "PRIMARY KEY";
        case Type::NOT_NULL:
            return "NOT NULL";
        case Type::FOREIGN_KEY:
            return "FOREIGN KEY";
        case Type::UNIQUE:
            return "UNIQUE";
        case Type::DEFAULT:
            return "DEFAULT";
        default:
            throw std::invalid_argument("Invalid Constraint Type enum value");
    }
}

