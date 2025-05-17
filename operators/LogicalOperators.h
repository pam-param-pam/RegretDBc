#pragma once

#include <memory>
#include <optional>
#include "Condition.h"
#include <functional>

class Condition;  // Forward declaration

class LogicalOperator {
public:
    virtual ~LogicalOperator() = default;

    [[nodiscard]] virtual std::optional<bool> evaluate(const TypeHints::Row &row) const = 0;

    [[nodiscard]] virtual std::string toString() const = 0;

    virtual void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const = 0;

};

class AND : public LogicalOperator {
public:
    AND(std::shared_ptr<Condition> left, std::shared_ptr<Condition> right);

    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row &row) const override;

    [[nodiscard]] std::string toString() const override;

    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const override;

private:
    std::shared_ptr<Condition> leftOperand;
    std::shared_ptr<Condition> rightOperand;

};

class OR : public LogicalOperator {
public:
    OR(std::shared_ptr<Condition> left, std::shared_ptr<Condition> right);

    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row &row) const override;

    [[nodiscard]] std::string toString() const override;

    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const override;

private:
    std::shared_ptr<Condition> leftOperand;
    std::shared_ptr<Condition> rightOperand;
};

class NOT : public LogicalOperator {
public:
    explicit NOT(std::shared_ptr<Condition> operand);

    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row &row) const override;

    [[nodiscard]] std::string toString() const override;

    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const override;

private:
    std::shared_ptr<Condition> operand;
};
