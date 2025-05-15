#pragma once

#include <memory>
#include <optional>
#include <functional>
#include "../TypeHints.h"
#include "../tokenTypes/Literal.h"
#include "ComparisonOperators.h"
#include "../exceptions/exceptions.h"
#include "LogicalOperators.h"
#include "NullOperators.h"

class LogicalOperator;  // Forward declaration

class Operand {
public:
    enum class Type {
        Boolean,
        ComparisonOperator,
        NullOperator,
        LogicalOperator
    };

    explicit Operand(bool value);
    explicit Operand(std::shared_ptr<ComparisonOperator> comparisonOp);
    explicit Operand(std::shared_ptr<IsNullCheck> nullOp);
    explicit Operand(std::shared_ptr<LogicalOperator> logicalOp);
    //https://www.geeksforgeeks.org/std-function-in-cpp/
    void visitColumns(const std::function<void(std::string &, const std::optional<Literal> &)>& visitor) const;

    [[nodiscard]] Type getType() const;
    [[nodiscard]] std::string toString() const;

    [[nodiscard]] std::optional<bool> evaluate(const TypeHints::Row& row) const;

private:
    Type type;
    bool boolValue{};
    std::shared_ptr<ComparisonOperator> comparisonOp;
    std::shared_ptr<IsNullCheck> nullOp;
    std::shared_ptr<LogicalOperator> logicalOp;
};
