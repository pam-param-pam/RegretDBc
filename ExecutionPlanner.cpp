#include "ExecutionPlanner.h"
#include "PlanNodes/CreateTablePlan.h"
#include "PlanNodes/InsertPlan.h"

std::shared_ptr<PlanNodeBase> ExecutionPlanner::plan(const std::shared_ptr<ASTNode> &statement) {
    fmt::println("ExecutionPlanner: plan");

    if (auto createAST = std::dynamic_pointer_cast<CreateAST>(statement)) {
        CreateTablePlan plan = CreateTablePlan(createAST->name, createAST->qualifiedColumns, createAST->columnTypes);
        return std::make_shared<CreateTablePlan>(plan);

    } else if (auto insertAST = std::dynamic_pointer_cast<InsertAST>(statement)) {
        InsertPlan plan = InsertPlan(insertAST->getTableName(), insertAST->getQualifiedColumns(), insertAST->values);
        return std::make_shared<InsertPlan>(plan);
    }

    if (auto selectAST = std::dynamic_pointer_cast<SelectAST>(statement)) {
//        selectAST->whereExpr->evaluate()

    }
//        // Step 1: TableScans
//        std::vector<std::shared_ptr<TableScan>> scans;
//        for (const auto& table : select_stmt->tables) {
//            scans.push_back(std::make_shared<TableScan>(table));
//        }
//
//        // Step 2: Build cross joins
//        std::shared_ptr<PlanNode> plan = scans[0];
//        for (size_t i = 1; i < scans.size(); ++i) {
//            plan = std::make_shared<CrossJoin>(plan, scans[i]);
//        }
//
//        // Step 3: WHERE clause
//        if (!select_stmt->where_expr.empty()) {
//            plan = std::make_shared<Filter>(plan, select_stmt->where_expr);
//        }
//
//        // Step 4: SELECT columns
//        plan = std::make_shared<Project>(plan, select_stmt->columns);
//
//        // Step 5: ORDER BY
//        if (!select_stmt->order_by.empty()) {
//            plan = std::make_shared<Sort>(plan, select_stmt->order_by);
//        }
//
//        // Step 6: Visualize
//        plan = std::make_shared<Visualize>(plan);
//
//        return plan;
//    }

//    else if (auto update_stmt = std::dynamic_pointer_cast<UpdateStmt>(statement)) {
//        // Step 1: Scan the target table
//        auto scan = std::make_shared<TableScan>(update_stmt->table);
//
//        // Step 2: Filter rows using WHERE clause
//        std::shared_ptr<PlanNode> plan = scan;
//        if (!update_stmt->where_expr.empty()) {
//            plan = std::make_shared<Filter>(plan, update_stmt->where_expr);
//        }
//
//        // Step 3: Apply Update operations
//        plan = std::make_shared<Update>(plan, update_stmt->assignments, update_stmt->table);
//
//        return plan;
//    }
//    else if (auto delete_stmt = std::dynamic_pointer_cast<DeleteStmt>(statement)) {
//        // Step 1: Scan the target table
//        auto scan = std::make_shared<TableScan>(delete_stmt->table);
//
//        // Step 2: Filter rows using WHERE clause
//        std::shared_ptr<PlanNode> plan = scan;
//        if (!delete_stmt->where_expr.empty()) {
//            plan = std::make_shared<Filter>(plan, delete_stmt->where_expr);
//        }
//
//        // Step 3: Apply Delete operations
//        plan = std::make_shared<Delete>(plan, delete_stmt->table, delete_stmt->where_expr);
//
//        return plan;
//    }
//    else if (auto drop_stmt = std::dynamic_pointer_cast<DropStmt>(statement)) {
//        return std::make_shared<DropTable>(drop_stmt->table);
//    }
//    else if (auto alter_add_stmt = std::dynamic_pointer_cast<AlterAddStmt>(statement)) {
//        // Handle ALTER ADD logic
//    }
//    else if (auto alter_modify_stmt = std::dynamic_pointer_cast<AlterModifyStmt>(statement)) {
//        // Handle ALTER MODIFY logic
//    }
//    else if (auto alter_rename_stmt = std::dynamic_pointer_cast<AlterRenameStmt>(statement)) {
//        // Handle ALTER RENAME logic
//    }
//    else if (auto alter_drop_stmt = std::dynamic_pointer_cast<AlterDropStmt>(statement)) {
//        // Handle ALTER DROP logic
//    }
    else {
        throw RegretDBError("Unexpected statement type: ");
    }
}
