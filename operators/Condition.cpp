#include "Condition.h"

Condition::Condition(bool value)
        : type(Type::Boolean), boolValue(value) {}

Condition::Condition(std::shared_ptr<ComparisonOperator> comparisonOp)
        : type(Type::ComparisonOperator), comparisonOp(std::move(comparisonOp)) {}

Condition::Condition(std::shared_ptr<IsNullCheck> nullOp)
        : type(Type::NullOperator), nullOp(std::move(nullOp)) {}

Condition::Condition(std::shared_ptr<LogicalOperator> logicalOp)
        : type(Type::LogicalOperator), logicalOp(std::move(logicalOp)) {}

Condition::Type Condition::getType() const {
    return type;
}

std::optional<bool> Condition::evaluate(const TypeHints::Row &row) const {
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
            throw IntegrityError("Unexpected: unable to match value in CONDITION");
    }
}

std::string Condition::toString() const {
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
            throw IntegrityError("Unexpected CONDITION type: " + std::to_string(static_cast<int>(type)));
    }
}

void Condition::visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const {
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
            throw IntegrityError("Unknown CONDITION type in visitColumns.");
    }
}
