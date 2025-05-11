#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"
#include "../tokenTypes/Literal.h"

class InsertAST : public ASTNode {
public:
    Identifier table;
    std::vector<Identifier> columns;
    std::vector<Literal> values;

    InsertAST(Identifier table, const std::vector<Identifier>& columns, const std::vector<Literal>& values);
    void performChecks() override;

    [[nodiscard]] std::string repr() const;

    [[nodiscard]] const std::string &getModifiedTableName() const;
    [[nodiscard]] const std::vector<std::string> &getModifiedQualifiedColumns() const;

private:
    std::string tableName;
    std::vector<std::string> qualifiedColumns;
};
