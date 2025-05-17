#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "../exceptions/exceptions.h"
#include "../tokenTypes/Identifier.h"
#include "../tokenTypes/Literal.h"
#include "../TypeHints.h"

class PlanNodeBase {
public:
    virtual ~PlanNodeBase() = default;

    virtual void execute() = 0;

    [[nodiscard]] virtual std::string toString(int level) const = 0;

    [[nodiscard]] virtual TypeHints::TableData getResult() const = 0;

protected:
    TypeHints::TableData resultData;
};
