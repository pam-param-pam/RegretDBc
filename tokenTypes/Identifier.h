#pragma once

#include <string>
#include <iostream>


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




