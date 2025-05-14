#pragma once

#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

class DropAST : public ASTNode {
public:
    Identifier table;

    explicit DropAST(Identifier table);

    void performChecks() override;
    [[nodiscard]] std::string repr() const override;
    [[nodiscard]] const std::string &getTableName() const;

private:
    std::string tableName;
};
