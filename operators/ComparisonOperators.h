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

    ComparisonOperator(std::string columnName, const Literal &literal)
            : columnName(std::move(columnName)), literal(literal) {}

    virtual ~ComparisonOperator() = default;

    [[nodiscard]] virtual std::optional<bool> evaluate(const Row &row) const = 0;

    [[nodiscard]] virtual std::string toString() const = 0;

    static std::shared_ptr<ComparisonOperator> fromLiteral(const std::string &op, const Identifier &left, const Literal &right);

    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor);

protected:
    std::string columnName;
    Literal literal;

    [[nodiscard]] std::pair<Literal, Literal> resolveAndUnpackOrThrow(const Row &row) const;

    [[nodiscard]] std::optional<Literal> resolve(const Row &row) const;

};


class EQ : public ComparisonOperator {
public:
    EQ(const std::string &column, const Literal &literal);

    [[nodiscard]] std::optional<bool> evaluate(const Row &row) const override;

    [[nodiscard]] std::string toString() const override;
};

class NEQ : public ComparisonOperator {
public:
    NEQ(const std::string &column, const Literal &literal);

    [[nodiscard]] std::optional<bool> evaluate(const Row &row) const override;

    [[nodiscard]] std::string toString() const override;
};

class LT : public ComparisonOperator {
public:
    LT(const std::string &column, const Literal &literal);

    [[nodiscard]] std::optional<bool> evaluate(const Row &row) const override;

    [[nodiscard]] std::string toString() const override;
};

class LTE : public ComparisonOperator {
public:
    LTE(const std::string &column, const Literal &literal);

    [[nodiscard]] std::optional<bool> evaluate(const Row &row) const override;

    [[nodiscard]] std::string toString() const override;
};

class GT : public ComparisonOperator {
public:
    GT(const std::string &column, const Literal &literal);

    [[nodiscard]] std::optional<bool> evaluate(const Row &row) const override;

    [[nodiscard]] std::string toString() const override;
};

class GTE : public ComparisonOperator {
public:
    GTE(const std::string &column, const Literal &literal);

    [[nodiscard]] std::optional<bool> evaluate(const Row &row) const override;

    [[nodiscard]] std::string toString() const override;
};
