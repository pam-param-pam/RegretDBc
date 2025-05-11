#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "../exceptions/exceptions.h"
#include "../tokenTypes/Identifier.h"
#include "../tokenTypes/Literal.h"

class PlanNodeBase {
public:
    virtual ~PlanNodeBase() = default;

    virtual void execute() = 0;

};
