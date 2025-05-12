#include "DeleteAST.h"


DeleteAST::DeleteAST(Identifier  table, std::optional<Operand> whereExpr)
        : table(std::move(table)), whereExpr(std::move(whereExpr)) {}

void DeleteAST::performChecks() {
    tableName = checkTable(table);
    checkWhereExpr({tableName}, whereExpr);
}

const std::string& DeleteAST::getTableName() const {
    return tableName;
}

std::string DeleteAST::repr() const {
    return "DeleteStmt(table=" + table.value + ", where=" + whereExpr->toString() + ")";
}
