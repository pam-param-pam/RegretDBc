#include "DropAST.h"


DropAST::DropAST(Identifier  table)
        : table(std::move(table)) {}

void DropAST::performChecks() {
    tableName = checkTable(table);
}

std::string DropAST::repr() const {
    return "DropStmt(table=" + table.value + ")";
}
