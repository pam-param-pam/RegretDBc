#pragma once

#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

class DeleteAST : public ASTNode {
public:
    DeleteAST(Identifier  table, std::string  where_expr);

    void performChecks() override;
    [[nodiscard]] std::string repr() const override;
    [[nodiscard]] const std::string &getTableName() const;

private:
    std::string tableName;
    Identifier table;
    std::string where_expr;
};
