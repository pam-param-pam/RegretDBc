#pragma once

#include <string>
#include <optional>
#include "../TypeHints.h"
#include "../tokenTypes/Literal.h"
#include "../exceptions/exceptions.h"


////------------ BASE CLASS ------------
class IsNullCheck {
public:
    using Row = TypeHints::Row;
    using Value = TypeHints::Value;

    explicit IsNullCheck(std::string columnName);

    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const;
    [[nodiscard]] std::string toString() const;

protected:
    std::string columnName;

    [[nodiscard]] std::optional<Value> resolve(const Row& row) const;
    [[nodiscard]] virtual bool isNullCheck() const = 0;
};


////------------ IS NULL ------------
class ISNULL : public IsNullCheck {
public:
    explicit ISNULL(std::string columnName);
protected:
    [[nodiscard]] bool isNullCheck() const override;
};


////------------ IS NOT NULL ------------
class ISNOTNULL : public IsNullCheck {
public:
    explicit ISNOTNULL(std::string columnName);
protected:
    [[nodiscard]] bool isNullCheck() const override;
};