#include "DeleteAST.h"


DeleteAST::DeleteAST(Identifier  table, std::string  where_expr)
        : table(std::move(table)), where_expr(std::move(where_expr)) {}

void DeleteAST::performChecks() {
    tableName = checkTable(table);
    //todo where_expr
}
const std::string& DeleteAST::getTableName() const {
    return tableName;
}

std::string DeleteAST::repr() const {
    return "DeleteStmt(table=" + table.value + ", where=" + where_expr + ")";
}
