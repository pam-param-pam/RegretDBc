#include "Operand.h"
#include "NullOperators.h"

Operand::Operand(bool value)
        : type(Type::Boolean), boolValue(value) {}

Operand::Operand(std::shared_ptr<ComparisonOperator> comparisonOp)
        : type(Type::ComparisonOperator), comparisonOp(std::move(comparisonOp)) {}

Operand::Operand(std::shared_ptr<IsNullCheck> nullOp)
        : type(Type::NullOperator), nullOp(std::move(nullOp)) {}

Operand::Operand(std::shared_ptr<LogicalOperator> logicalOp)
        : type(Type::LogicalOperator), logicalOp(std::move(logicalOp)) {}

Operand::Type Operand::getType() const {
    return type;
}

std::optional<bool> Operand::evaluate(const TypeHints::Row &row) const {
    switch (type) {
        case Type::Boolean:
            return boolValue;
        case Type::ComparisonOperator:
            return comparisonOp->evaluate(row);
        case Type::NullOperator:
            return nullOp->evaluate(row);
        case Type::LogicalOperator:
            return logicalOp->evaluate(row);
        default:
            throw IntegrityError("Unexpected: unable to match value in OPERAND");
    }
}

std::string Operand::toString() const {
    switch (type) {
        case Type::Boolean:
            return boolValue ? "True" : "False";
        case Type::ComparisonOperator:
            return comparisonOp->toString();
        case Type::NullOperator:
            return nullOp->toString();
        case Type::LogicalOperator:
            return logicalOp->toString();
        default:
            throw IntegrityError("Unexpected Operand type: " + std::to_string(static_cast<int>(type)));
    }
}

void Operand::visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const {
    switch (type) {
        case Type::Boolean:
            // No columns to visit in a boolean literal
            break;
        case Type::ComparisonOperator:
            comparisonOp->visitColumns(visitor);
            break;
        case Type::NullOperator:
            nullOp->visitColumns(visitor);
            break;
        case Type::LogicalOperator:
            logicalOp->visitColumns(visitor);
            break;
        default:
            throw IntegrityError("Unknown operand type in visitColumns.");
    }
}
