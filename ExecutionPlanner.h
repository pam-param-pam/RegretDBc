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
//#include "PlanNodes/CreatePlanNode.h"
//#include "PlanNodes/SelectPlanNodes.h"
//#include "PlanNodes/InsertPlanNode.h"
//#include "PlanNodes/UpdatePlanNode.h"
//#include "PlanNodes/DeletePlanNode.h"
//#include "PlanNodes/DropTablePlanNode.h"
//
class ExecutionPlanner {
public:
    std::shared_ptr<PlanNodeBase> plan(const std::shared_ptr<ASTNode>& statement);
};
