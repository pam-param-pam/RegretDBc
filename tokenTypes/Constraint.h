#pragma once

#include <string>
#include <optional>
#include <iostream>

class Constraint {
public:
    enum class Type {
        PRIMARY_KEY,
        NOT_NULL,
        FOREIGN_KEY,
        UNIQUE,
        DEFAULT
    };

    explicit Constraint(Type type, std::optional<std::string> arg1 = std::nullopt, std::optional<std::string> arg2 = std::nullopt);
    [[nodiscard]] std::string toString() const;

private:
    Type type;
    std::optional<std::string> arg1;
    std::optional<std::string> arg2;

    static std::string typeToString(Type type);
};

