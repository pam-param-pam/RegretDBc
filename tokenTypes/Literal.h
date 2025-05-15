#pragma once

#include <string>
#include <iostream>
#include <variant>
#include <optional>
///holds_alternative is a such a dumb name for checking type of a variant...
///todo: This shouldn't really be called Literal, since it's now used in dataManager::Row


class Literal {
public:
    enum class Type {
        TEXT,
        INTEGER,
        BOOLEAN,
        NULL_VALUE,
    };

    using Value = std::variant<std::monostate, std::string, int, bool>;  //unable to use TypeHints due to I think circular include
    Literal(const Literal& other) = default;
    Literal(Type type, Value value);
    Literal() : type(Type::NULL_VALUE), value(std::monostate{}) {}

    explicit Literal(const Value& val);

    [[nodiscard]] std::string toString() const;
    [[nodiscard]] Value getValue() const;
    [[nodiscard]] Type getType() const;
    static Type getTypeFromValue(const std::string& tokenType);
    static std::string typeToString(Type type);

    bool operator==(const Literal& other) const;

    [[nodiscard]] bool isNull() const;

private:
    Type type;
    Value value;
};
