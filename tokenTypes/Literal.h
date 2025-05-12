#pragma once

#include <string>
#include <iostream>
#include <variant>
#include <optional>

class Literal {
public:
    enum class Type {
        TEXT,
        INTEGER,
        BOOLEAN,
        NULL_VALUE,
    };

    using Value = std::variant<std::monostate, std::string, int, bool>;  //unable to use TypeHints due to I think circular include

    Literal(Type type, Value value, std::optional<size_t> size = std::nullopt);

    [[nodiscard]] std::string toString() const;
    [[nodiscard]] Value getValue() const;
    [[nodiscard]] Type getType() const;
    static Type getTypeFromValue(const std::string& tokenType);
    static std::string typeToString(Type type);


private:
    Type type;
    Value value;
    std::optional<size_t> size;

};
