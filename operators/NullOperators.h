#pragma once

#include <string>
#include <optional>
#include <functional>
#include "../TypeHints.h"
#include "../tokenTypes/Literal.h"
#include "../exceptions/exceptions.h"


///------------ BASE CLASS ------------
class IsNullCheck {
public:
    using Row = TypeHints::Row;

    explicit IsNullCheck(std::string columnName);

    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const;
    [[nodiscard]] std::string toString() const;
    void visitColumns(const std::function<void(std::string&, const std::optional<Literal>&)>& visitor);

protected:
    std::string columnName;

    [[nodiscard]] Literal resolve(const Row& row) const;
    [[nodiscard]] virtual bool isNullCheck() const = 0;
};


///------------ IS NULL ------------
class ISNULL : public IsNullCheck {
public:
    explicit ISNULL(std::string columnName);
protected:
    [[nodiscard]] bool isNullCheck() const override;
};


///------------ IS NOT NULL ------------
class ISNOTNULL : public IsNullCheck {
public:
    explicit ISNOTNULL(std::string columnName);
protected:
    [[nodiscard]] bool isNullCheck() const override;
};
