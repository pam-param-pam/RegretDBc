#pragma once

#include "PlanNodes/PlanNodeBase.h"
#include "ASTNodes/ASTNodeBase.h"

class ExecutionPlanner {
public:
    std::unique_ptr<PlanNodeBase> plan(const std::unique_ptr<ASTNode> &statement);
};
