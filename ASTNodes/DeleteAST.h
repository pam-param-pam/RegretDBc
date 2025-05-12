#pragma once

#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

class DeleteAST : public ASTNode {
public:
    DeleteAST(Identifier table, std::optional<Operand> whereExpr);

    void performChecks() override;
    [[nodiscard]] std::string repr() const override;
    [[nodiscard]] const std::string &getTableName() const;

    std::optional<Operand> whereExpr;
private:
    std::string tableName;
    Identifier table;


};
