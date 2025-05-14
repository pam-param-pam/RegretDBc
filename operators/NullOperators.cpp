#include "NullOperators.h"
#include "fmt/base.h"

#include <utility>

///------------ BASE CLASS ------------
IsNullCheck::IsNullCheck(std::string columnName)
        : columnName(std::move(columnName)) {}

std::string IsNullCheck::toString() const {
    return (isNullCheck() ? "IS_NULL(" : "IS_NOT_NULL(") + columnName + ")";
}

IsNullCheck::Value IsNullCheck::resolve(const Row& row) const {
    auto it = row.find(columnName);
    if (it == row.end()) {
        throw IntegrityError("Unexpected: Column '" + columnName + "' not found in row");
    }
    return it->second;

}

std::optional<bool> IsNullCheck::evaluate(const IsNullCheck::Row &row) const {
    auto value = resolve(row);
    bool isNull = std::holds_alternative<std::monostate>(value);
    return isNullCheck() ? isNull : !isNull;

}

void IsNullCheck::visitColumns(const std::function<void(std::string&, const std::optional<Literal> &)> &visitor) {
    visitor(columnName, std::nullopt);  // No literal available
}


///------------ IS NULL ------------
ISNULL::ISNULL(std::string columnName)
        : IsNullCheck(std::move(columnName)) {}

bool ISNULL::isNullCheck() const {
    return true;
}

///------------ IS NOT NULL ------------
ISNOTNULL::ISNOTNULL(std::string columnName)
        : IsNullCheck(std::move(columnName)) {}

bool ISNOTNULL::isNullCheck() const {
    return false;
}
