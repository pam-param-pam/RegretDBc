#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ASTNodes/SelectAST.h"
#include "ASTNodes/InsertAST.h"
#include "ASTNodes/UpdateAST.h"
#include "ASTNodes/DeleteAST.h"
#include "ASTNodes/CreateAST.h"
#include "ASTNodes/DropAST.h"
#include "exceptions/exceptions.h"

#include "PlanNodes/PlanNodeBase.h"

class ExecutionPlanner {
public:
    std::shared_ptr<PlanNodeBase> plan(const std::shared_ptr<ASTNode>& statement);
};
