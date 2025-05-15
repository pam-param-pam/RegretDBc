#pragma once

#include <string>
#include <variant>
#include <unordered_map>
#include <vector>
#include "tokenTypes/Literal.h"

class TypeHints {
public:
    using Row = std::unordered_map<std::string, Literal>;
    using TableData = std::vector<Row>;
    using ColumnTypeMap = std::unordered_map<std::string, Literal::Type>;

};
