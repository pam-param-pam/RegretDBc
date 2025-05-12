#pragma once

#include <string>
#include <vector>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"
#include "../tokenTypes/Literal.h"

class UpdateAST : public ASTNode {
public:
    UpdateAST(Identifier  table, const std::vector<std::pair<Identifier, Literal>>& assignments, std::optional<Operand> whereExpr);

    Identifier table;
    std::vector<std::pair<Identifier, Literal>> assignments;
    std::optional<Operand> whereExpr;

    void performChecks() override;
    [[nodiscard]] const std::string &getTableName() const;
    [[nodiscard]] const std::vector<std::pair<std::string, Literal>> &getQualifiedAssignments() const;

    [[nodiscard]] std::string repr() const override;
private:
    std::string tableName;
    std::vector<std::pair<std::string, Literal>> qualifiedAssignments;
};
