#pragma once

#include <string>

class Identifier {
public:
    enum class Type {
        COLUMN,
        TABLE
    };

    Identifier(Type type, std::string value);

    [[nodiscard]] std::string toString() const;

    Type type;
    std::string value;

private:
    static std::string typeToString(Type type);
};
