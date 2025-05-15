#include "ExecutionPlanner.h"
#include "PlanNodes/CreateTablePlan.h"
#include "PlanNodes/InsertPlan.h"
#include "PlanNodes/SelectPlans.h"
#include "PlanNodes/DeletePlan.h"
#include "PlanNodes/UpdatePlan.h"
#include "PlanNodes/DropTablePlan.h"
#include "ASTNodes/AlterAST.h"
#include "PlanNodes/AlterTablePlan.h"
#include "ASTNodes/DumpLoadAST.h"
#include "PlanNodes/DumpPlan.h"
#include "PlanNodes/LoadPlan.h"

std::shared_ptr<PlanNodeBase> ExecutionPlanner::plan(const std::shared_ptr<ASTNode> &statement) {

    if (auto createAST = std::dynamic_pointer_cast<CreateAST>(statement)) {
        CreateTablePlan plan = CreateTablePlan(createAST->getTableName(), createAST->getQualifiedColumns(), createAST->getColumnTypes());
        return std::make_shared<CreateTablePlan>(plan);

    } else if (auto insertAST = std::dynamic_pointer_cast<InsertAST>(statement)) {
        InsertPlan plan = InsertPlan(insertAST->getTableName(), insertAST->getQualifiedColumns(), insertAST->getValues());
        return std::make_shared<InsertPlan>(plan);
    }

    if (auto selectAST = std::dynamic_pointer_cast<SelectAST>(statement)) {

        std::vector<std::shared_ptr<PlanNodeBase>> scans;
        for (const auto& table : selectAST->getTableNames()) {
            scans.push_back(std::make_shared<TableScan>(table));
        }

        // Step 2: Chain them into CrossJoins
        std::shared_ptr<PlanNodeBase> plan = scans[0];
        for (auto i = 1; i < scans.size(); ++i) {
            plan = std::make_shared<CrossJoin>(plan, scans[i]);
        }

        // Step 3: WHERE clause
        if (selectAST->getWhereExpr()) {
            plan = std::make_shared<Filter>(plan, *selectAST->getWhereExpr());
        }

        // Step 4: SELECT columns
        plan = std::make_shared<Project>(plan, selectAST->getQualifiedColumns());

        // Step 5: ORDER BY
        auto orderBy = selectAST->getQualifiedOrderBy();
        if (!orderBy.empty()) {
            plan = std::make_shared<Sort>(plan, selectAST->getQualifiedOrderBy());
        }

        // Step 6: Visualize
        plan = std::make_shared<Visualize>(plan);

        return plan;
    }

    else if (auto deleteAST = std::dynamic_pointer_cast<DeleteAST>(statement)) {
        // Step 1: Scan the target table
        auto scan = std::make_shared<TableScan>(deleteAST->getTableName());

        // Step 2: Filter rows using WHERE clause
        std::shared_ptr<PlanNodeBase> plan = scan;
        if (deleteAST->getWhereExpr()) {
            plan = std::make_shared<Filter>(plan, *deleteAST->getWhereExpr());
        }

        // Step 3: Apply Delete operations
        plan = std::make_shared<DeletePlan>(plan, deleteAST->getTableName());

        return plan;
    }
    else if (auto updateAST = std::dynamic_pointer_cast<UpdateAST>(statement)) {

        // Step 1: Scan the target table
        auto scan = std::make_shared<TableScan>(updateAST->getTableName());

        // Step 2: Filter rows using WHERE clause
        std::shared_ptr<PlanNodeBase> plan = scan;

        if (updateAST->getWhereExpr()) {
            plan = std::make_shared<Filter>(plan, *updateAST->getWhereExpr());
        }

        // Step 3: Apply Update operations
        plan = std::make_shared<UpdatePlan>(plan, updateAST->getTableName(), updateAST->getQualifiedAssignments());

        return plan;
    }

    else if (auto dropAST = std::dynamic_pointer_cast<DropAST>(statement)) {
        return std::make_shared<DropTablePlan>(dropAST->getTableName());
    }
    else if (auto alterAST = std::dynamic_pointer_cast<AlterAST>(statement)) {
        return std::make_shared<AlterTablePlan>(alterAST->getAction(), alterAST->getTableName(), alterAST->getQualifiedColumn(), alterAST->getNewValue());
    }
    else if (auto dumpLoadAST = std::dynamic_pointer_cast<DumpLoadAST>(statement)) {
        if (dumpLoadAST->getType() == DumpLoadAST::Type::DUMP) {
            return std::make_shared<DumpPlan>(dumpLoadAST->getFilePath());
        } else if (dumpLoadAST->getType() == DumpLoadAST::Type::LOAD) {
            return std::make_shared<LoadPlan>(dumpLoadAST->getFilePath());
        } else throw RegretDBError("Unexpected dumpLoadAST type in ExecutionPlanner");
    }

    else {
        throw RegretDBError("Unexpected statement type in ExecutionPlanner");
    }
}
