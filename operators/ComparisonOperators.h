#pragma once

#include <string>
#include <optional>
#include <functional>
#include "../TypeHints.h"
#include "../tokenTypes/Literal.h"
#include "../exceptions/exceptions.h"
#include "../tokenTypes/Identifier.h"


///BASE CLASS
class ComparisonOperator {
public:
    using Row = TypeHints::Row;
    using Value = TypeHints::Value;

    ComparisonOperator(std::string columnName, Literal literal)
            : columnName(std::move(columnName)), literal(std::move(literal)) {}

    virtual ~ComparisonOperator() = default;

    [[nodiscard]] virtual std::optional<bool> evaluate(const Row& row) const = 0;
    [[nodiscard]] virtual std::string toString() const = 0;

    static std::shared_ptr<ComparisonOperator> fromLiteral(const std::string &op, const Identifier &left, const Literal &right);
    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor);

protected:
    std::string columnName;
    Literal literal;

    [[nodiscard]] std::pair<Value, Value> resolveAndUnpackOrThrow(const Row& row) const;

    [[nodiscard]] std::optional<Value> resolve(const Row &row) const;

};


class EQ : public ComparisonOperator {
public:
    EQ(const std::string& column, const Literal& literal);
    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const override;
    [[nodiscard]] std::string toString() const override;
};

class NEQ : public ComparisonOperator {
public:
    NEQ(const std::string& column, const Literal& literal);
    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const override;
    [[nodiscard]] std::string toString() const override;
};

class LT : public ComparisonOperator {
public:
    LT(const std::string& column, const Literal& literal);
    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const override;
    [[nodiscard]] std::string toString() const override;
};

class LTE : public ComparisonOperator {
public:
    LTE(const std::string& column, const Literal& literal);
    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const override;
    [[nodiscard]] std::string toString() const override;
};

class GT : public ComparisonOperator {
public:
    GT(const std::string& column, const Literal& literal);
    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const override;
    [[nodiscard]] std::string toString() const override;
};

class GTE : public ComparisonOperator {
public:
    GTE(const std::string& column, const Literal& literal);
    [[nodiscard]] std::optional<bool> evaluate(const Row& row) const override;
    [[nodiscard]] std::string toString() const override;
};

