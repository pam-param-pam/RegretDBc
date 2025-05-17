#include "UpdateAST.h"
#include "fmt/format.h"

UpdateAST::UpdateAST(Identifier table, const std::vector<std::pair<Identifier, Literal>> &assignments, std::optional<Condition> whereExpr)
        : table(std::move(table)), assignments(assignments), whereExpr(std::move(whereExpr)) {}

void UpdateAST::performChecks() {
    tableName = checkTable(table);

    if (whereExpr.has_value()) checkWhereExpr({tableName}, whereExpr);

    const auto &colTypesMap = DataManager::getInstance().getColumnTypesForTable(tableName);

    for (const auto &[col, literal]: assignments) {
        auto qualifiedColumn = checkColumn({tableName}, col.value);
        checkColumnType(colTypesMap, qualifiedColumn, literal);
        qualifiedAssignments.emplace_back(qualifiedColumn, literal);
    }
}

const std::string &UpdateAST::getTableName() const {
    return tableName;
}

const std::vector<std::pair<std::string, Literal>> &UpdateAST::getQualifiedAssignments() const {
    return qualifiedAssignments;
}

std::string UpdateAST::repr() const {
    std::string assignmentStr;
    for (auto i = 0; i < assignments.size(); ++i) {
        assignmentStr += fmt::format("({}, {})", assignments[i].first.value, assignments[i].second.toString());
        if (i < assignments.size() - 1) {
            assignmentStr += ", ";
        }
    }
    return fmt::format("UpdateStmt(table={}, assignments=[{}], where={})",
                       table.value, assignmentStr,
                       whereExpr ? whereExpr->toString() : "None");
}

const std::optional<Condition> &UpdateAST::getWhereExpr() const {
    return whereExpr;
}
