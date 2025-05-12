#pragma once

#include <memory>
#include <optional>
#include "Operand.h"

class Operand;  // Forward declaration

class LogicalOperator {
public:
    virtual ~LogicalOperator() = default;
    [[nodiscard]] virtual std::optional<bool> evaluate(const TypeHints::Row& row) const = 0;
    [[nodiscard]] virtual std::string toString() const = 0;
};

class AND: public LogicalOperator {
public:
    AND(std::shared_ptr<Operand> left, std::shared_ptr<Operand> right);
    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row& row) const override;
    [[nodiscard]] std::string toString() const override;

private:
    std::shared_ptr<Operand> leftOperand;
    std::shared_ptr<Operand> rightOperand;

};

class OR: public LogicalOperator {
public:
    OR(std::shared_ptr<Operand> left, std::shared_ptr<Operand> right);
    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row& row) const override;
    [[nodiscard]] std::string toString() const override;

private:
    std::shared_ptr<Operand> leftOperand;
    std::shared_ptr<Operand> rightOperand;
};

class NOT: public LogicalOperator {
public:
    explicit NOT(std::shared_ptr<Operand> operand);
    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row& row) const override;
    [[nodiscard]] std::string toString() const override;

private:
    std::shared_ptr<Operand> operand;
};
