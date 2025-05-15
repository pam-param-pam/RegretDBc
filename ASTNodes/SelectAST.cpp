#include "SelectAST.h"
#include "fmt/format.h"
#include "fmt/ranges.h"

SelectAST::SelectAST(const std::vector<Identifier> &columns, const std::vector<Identifier> &tables, std::optional<Operand> whereExpr,
                     std::vector<std::pair<Identifier, std::string>> orderBy)
        : columns(columns), tables(tables), whereExpr(std::move(whereExpr)), orderBy(std::move(orderBy)) {
}

void SelectAST::performChecks() {
    tableNames = checkTables(tables);


    for (const auto& col : columns) {
        if (col.value == "*") {
            for (const auto& table : tableNames) {
                const auto& tableColumns = DataManager::getInstance().getColumnsForTable(table);
                for (const auto& expandedColumn : tableColumns) {
                    qualifiedColumns.push_back(checkColumn(tableNames, expandedColumn));
                }
            }
        } else {
            qualifiedColumns.push_back(checkColumn(tableNames, col.value));
        }
    }

    if (whereExpr.has_value()) checkWhereExpr(tableNames, whereExpr);

    //checking order by:
    for (const auto& [identifier, direction] : orderBy) {
        auto qualifiedColumn = checkColumn(tableNames, identifier.value);

        bool isDescending = (direction == "DESC");

        qualifiedOrderBy.emplace_back(qualifiedColumn, isDescending);
    }
}

const std::vector<std::string>& SelectAST::getTableNames() const {
    return tableNames;
}

const std::vector<std::string>& SelectAST::getQualifiedColumns() const {
    return qualifiedColumns;
}

const std::vector<std::pair<std::string, bool>> &SelectAST::getQualifiedOrderBy() const {
    return qualifiedOrderBy;
}
const std::optional<Operand> &SelectAST::getWhereExpr() const {
    return whereExpr;
}
std::string SelectAST::repr() const {
    std::vector<std::string> columnReprs;
    columnReprs.reserve(columns.size());
    for (const auto &col: columns) {
        columnReprs.push_back(col.toString());
    }

    std::vector<std::string> tableReprs;
    tableReprs.reserve(tables.size());
    for (const auto &tbl: tables) {
        tableReprs.push_back(tbl.toString());
    }

    std::vector<std::string> orderByReprs;
    orderByReprs.reserve(orderBy.size());
    for (const auto& [identifier, direction] : orderBy) {
        orderByReprs.push_back(fmt::format("({}, {})", identifier.toString(), direction));
    }

    return fmt::format("SelectStmt(columns=[{}], tables=[{}], where={}, order_by=[{}])",
                       fmt::join(columnReprs, ", "),
                       fmt::join(tableReprs, ", "),
                       whereExpr ? whereExpr->toString() : "None",
                       fmt::join(orderByReprs, ", "));
}
