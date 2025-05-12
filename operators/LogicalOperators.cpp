#include "LogicalOperators.h"

AND::AND(std::shared_ptr<Operand> left, std::shared_ptr<Operand> right)
        : leftOperand(std::move(left)), rightOperand(std::move(right)) {}

std::optional<bool> AND::evaluate(const TypeHints::Row& row) const {
    auto leftEval = leftOperand->evaluate(row);
    auto rightEval = rightOperand->evaluate(row);

    if (leftEval.has_value() && rightEval.has_value()) {
        return leftEval.value() && rightEval.value();
    }
    return std::nullopt; // If either operand cannot be evaluated, return nullopt
}


void AND::visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const {
    leftOperand->visitColumns(visitor);
    rightOperand->visitColumns(visitor);
}
std::string AND::toString() const {
    return "AND(" + leftOperand->toString() + ", " + rightOperand->toString() + ")";
}


// OR implementation
OR::OR(std::shared_ptr<Operand> left, std::shared_ptr<Operand> right)
        : leftOperand(std::move(left)), rightOperand(std::move(right)) {}

std::optional<bool> OR::evaluate(const TypeHints::Row& row) const {
    auto leftEval = leftOperand->evaluate(row);
    auto rightEval = rightOperand->evaluate(row);

    if (leftEval.has_value() && rightEval.has_value()) {
        return leftEval.value() || rightEval.value();
    }
    return std::nullopt; // If either operand cannot be evaluated, return nullopt
}

void OR::visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const {
    leftOperand->visitColumns(visitor);
    rightOperand->visitColumns(visitor);
}

std::string OR::toString() const {
    return "OR(" + leftOperand->toString() + ", " + rightOperand->toString() + ")";
}

// NOT implementation
NOT::NOT(std::shared_ptr<Operand> operand)
        : operand(std::move(operand)) {}

std::optional<bool> NOT::evaluate(const TypeHints::Row& row) const {
    auto operandEval = operand->evaluate(row);

    if (operandEval.has_value()) {
        return !operandEval.value();
    }
    return std::nullopt; // If operand cannot be evaluated, return nullopt
}

void NOT::visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const {
    operand->visitColumns(visitor);
}

std::string NOT::toString() const {
    return "NOT(" + operand->toString() + ")";
}
