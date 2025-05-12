#include "SelectAST.h"
#include "fmt/format.h"
#include "fmt/ranges.h"

#include <sstream>
#include <utility>


// Constructor definition
SelectAST::SelectAST(const std::vector<Identifier> &columns, const std::vector<Identifier> &tables, std::shared_ptr<Operand> whereExpr, std::string order_by)
        : columns(columns), tables(tables), whereExpr(std::move(whereExpr)), order_by(std::move(order_by)) {
}

void SelectAST::performChecks() {
    tableNames = checkTables(tables);
    qualifiedColumns = checkColumns(tableNames, columns);
    //todo validate where clause, and orderBy clause
}

const std::vector<std::string>& SelectAST::getTableNames() const {
    return tableNames;
}

const std::vector<std::string>& SelectAST::getQualifiedColumns() const {
    return qualifiedColumns;
}

std::string SelectAST::repr() const {
    std::vector<std::string> column_reprs;
    column_reprs.reserve(columns.size());
    for (const auto &col: columns) {
        column_reprs.push_back(col.toString());
    }

    std::vector<std::string> table_reprs;
    table_reprs.reserve(tables.size());
    for (const auto &tbl: tables) {
        table_reprs.push_back(tbl.toString());
    }

    return fmt::format("SelectStmt(columns=[{}], tables=[{}], where={}, order_by={})",
                       fmt::join(column_reprs, ", "),
                       fmt::join(table_reprs, ", "),
                       whereExpr->toString(), order_by);
}


