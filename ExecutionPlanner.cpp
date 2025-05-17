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
#include "ASTNodes/CreateAST.h"
#include "ASTNodes/InsertAST.h"
#include "ASTNodes/SelectAST.h"
#include "ASTNodes/DeleteAST.h"
#include "ASTNodes/DropAST.h"
#include "ASTNodes/UpdateAST.h"

std::unique_ptr<PlanNodeBase> ExecutionPlanner::plan(const std::unique_ptr<ASTNode> &statement) {

    if (auto *createAST = dynamic_cast<CreateAST *>(statement.get())) {
        CreateTablePlan plan = CreateTablePlan(createAST->getTableName(), createAST->getQualifiedColumns(), createAST->getColumnTypes());
        return std::make_unique<CreateTablePlan>(plan);

    } else if (auto *insertAST = dynamic_cast<InsertAST *>(statement.get())) {
        InsertPlan plan = InsertPlan(insertAST->getTableName(), insertAST->getQualifiedColumns(), insertAST->getValues());
        return std::make_unique<InsertPlan>(plan);
    }

    if (auto *selectAST = dynamic_cast<SelectAST *>(statement.get())) {

        std::vector<std::unique_ptr<PlanNodeBase>> scans;
        for (const auto &table: selectAST->getTableNames()) {
            scans.push_back(std::make_unique<TableScan>(table));
        }

        // Step 2: Chain them into CrossJoins
        std::unique_ptr<PlanNodeBase> plan = std::move(scans[0]);
        for (auto i = 1; i < scans.size(); ++i) {
            // Move ownership of plan and scans[i] into new CrossJoin node
            plan = std::make_unique<CrossJoin>(std::move(plan), std::move(scans[i]));
        }

        // Step 3: WHERE clause
        if (selectAST->getWhereExpr()) {
            plan = std::make_unique<Filter>(std::move(plan), *selectAST->getWhereExpr());
        }

        // Step 4: SELECT columns
        plan = std::make_unique<Project>(std::move(plan), selectAST->getQualifiedColumns());

        // Step 5: ORDER BY
        auto orderBy = selectAST->getQualifiedOrderBy();
        if (!orderBy.empty()) {
            plan = std::make_unique<Sort>(std::move(plan), selectAST->getQualifiedOrderBy());
        }

        // Step 6: Visualize
        plan = std::make_unique<Visualize>(std::move(plan));

        return plan;
    } else if (auto *deleteAST = dynamic_cast<DeleteAST *>(statement.get())) {
        // Step 1: Scan the target table
        std::unique_ptr<PlanNodeBase> plan = std::make_unique<TableScan>(deleteAST->getTableName());

        if (deleteAST->getWhereExpr()) {
            plan = std::make_unique<Filter>(std::move(plan), *deleteAST->getWhereExpr());
        }

        // Step 3: Apply Delete operations
        plan = std::make_unique<DeletePlan>(std::move(plan), deleteAST->getTableName());

        return plan;
    } else if (auto *updateAST = dynamic_cast<UpdateAST *>(statement.get())) {

        // Step 1: Scan the target table
        std::unique_ptr<PlanNodeBase> plan = std::make_unique<TableScan>(updateAST->getTableName());

        // Step 2: Filter rows using WHERE clause
        if (updateAST->getWhereExpr()) {
            plan = std::make_unique<Filter>(std::move(plan), *updateAST->getWhereExpr());
        }

        // Step 3: Apply Update operations
        plan = std::make_unique<UpdatePlan>(std::move(plan), updateAST->getTableName(), updateAST->getQualifiedAssignments());

        return plan;
    } else if (auto *dropAST = dynamic_cast<DropAST *>(statement.get())) {
        return std::make_unique<DropTablePlan>(dropAST->getTableName());
    } else if (auto *alterAST = dynamic_cast<AlterAST *>(statement.get())) {
        return std::make_unique<AlterTablePlan>(alterAST->getAction(), alterAST->getTableName(), alterAST->getQualifiedColumn(), alterAST->getNewValue());
    } else if (auto *dumpLoadAST = dynamic_cast<DumpLoadAST *>(statement.get())) {
        if (dumpLoadAST->getType() == DumpLoadAST::Type::DUMP) {
            return std::make_unique<DumpPlan>(dumpLoadAST->getFilePath());
        }
        if (dumpLoadAST->getType() == DumpLoadAST::Type::LOAD) {
            return std::make_unique<LoadPlan>(dumpLoadAST->getFilePath());
        }
        throw RegretDBError("Unexpected dumpLoadAST type in ExecutionPlanner");
    } else {
        throw RegretDBError("Unexpected statement type in ExecutionPlanner");
    }
}
