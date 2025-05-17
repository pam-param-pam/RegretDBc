#include "DeleteAST.h"


DeleteAST::DeleteAST(Identifier table, std::optional<Condition> whereExpr)
        : table(std::move(table)), whereExpr(std::move(whereExpr)) {}

void DeleteAST::performChecks() {
    tableName = checkTable(table);
    if (whereExpr.has_value()) checkWhereExpr({tableName}, whereExpr);
}

const std::string &DeleteAST::getTableName() const {
    return tableName;
}

const std::optional<Condition> &DeleteAST::getWhereExpr() const {
    return whereExpr;
}

std::string DeleteAST::repr() const {
    return "DeleteStmt(table=" + table.value + ", where=" + whereExpr->toString() + ")";
}
