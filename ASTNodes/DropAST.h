#pragma once

#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

class DropAST : public ASTNode {
public:
    explicit DropAST(Identifier table);

    void performChecks() override;

    [[nodiscard]] std::string repr() const override;

    [[nodiscard]] const std::string &getTableName() const;

private:
    Identifier table;

    std::string tableName;
};
