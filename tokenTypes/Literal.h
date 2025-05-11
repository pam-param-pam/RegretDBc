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
        BOOLEAN
    };

    using Value = std::variant<std::string, int, bool>;

    Literal(Type type, Value value, std::optional<size_t> size = std::nullopt);

    [[nodiscard]] std::string toString() const;
    Value getValue();
    Type getType();
    static Type getTypeFromValue(const std::string& tokenType);
    static std::string typeToString(Type type);


private:
    Type type;
    Value value;
    std::optional<size_t> size;

};
