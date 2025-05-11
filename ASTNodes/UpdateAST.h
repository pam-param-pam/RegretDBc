#pragma once

#include <string>
#include <vector>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"
#include "../tokenTypes/Literal.h"

class UpdateAST : public ASTNode {
public:
    Identifier table;
    std::vector<std::pair<Identifier, Literal>> assignments;
    std::string where_expr;

    UpdateAST(Identifier  table, const std::vector<std::pair<Identifier, Literal>>& assignments, std::string  where_expr);

    void performChecks() override;
    [[nodiscard]] std::string repr() const override;
};
