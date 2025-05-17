#pragma once

#include "../TypeHints.h"
#include "../tokenTypes/Literal.h"
#include "../exceptions/exceptions.h"
#include "LogicalOperators.h"
#include "NullOperators.h"
#include "ComparisonOperators.h"

class LogicalOperator;  // Forward declaration

class Condition {
public:
    enum class Type {
        Boolean,
        ComparisonOperator,
        NullOperator,
        LogicalOperator
    };

    explicit Condition(bool value);

    explicit Condition(std::shared_ptr<ComparisonOperator> comparisonOp);

    explicit Condition(std::shared_ptr<IsNullCheck> nullOp);

    explicit Condition(std::shared_ptr<LogicalOperator> logicalOp);

    //https://www.geeksforgeeks.org/std-function-in-cpp/
    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)> &visitor) const;

    [[nodiscard]] Type getType() const;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row &row) const;

private:
    Type type;
    bool boolValue{};
    std::shared_ptr<ComparisonOperator> comparisonOp;
    std::shared_ptr<IsNullCheck> nullOp;
    std::shared_ptr<LogicalOperator> logicalOp;
};
