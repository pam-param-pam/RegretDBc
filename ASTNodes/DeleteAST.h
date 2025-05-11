#pragma once

#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

class DeleteAST : public ASTNode {
public:
    Identifier table;
    std::string where_expr;

    DeleteAST(Identifier  table, std::string  where_expr);

    void performChecks() override;
    [[nodiscard]] std::string repr() const override;
};
