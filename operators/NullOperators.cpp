#include "NullOperators.h"

#include <utility>

////------------ BASE CLASS ------------
IsNullCheck::IsNullCheck(std::string columnName)
        : columnName(std::move(columnName)) {}

std::string IsNullCheck::toString() const {
    return columnName + (isNullCheck() ? " IS NULL" : " IS NOT NULL");
}

std::optional<IsNullCheck::Value> IsNullCheck::resolve(const Row& row) const {
    auto it = row.find(columnName);
    if (it == row.end()) {
        throw IntegrityError("Unexpected: Column '" + columnName + "' not found in row");
    }
    return it->second;

}

std::optional<bool> IsNullCheck::evaluate(const IsNullCheck::Row &row) const {
    auto value = resolve(row);
    if (value) {
        return !isNullCheck();
    }
    return isNullCheck();

}

////------------ IS NULL ------------
ISNULL::ISNULL(std::string columnName)
        : IsNullCheck(std::move(columnName)) {}

bool ISNULL::isNullCheck() const {
    return true;
}

////------------ IS NOT NULL ------------
ISNOTNULL::ISNOTNULL(std::string columnName)
        : IsNullCheck(std::move(columnName)) {}

bool ISNOTNULL::isNullCheck() const {
    return false;
}
