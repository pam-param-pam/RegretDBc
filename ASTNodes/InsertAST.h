#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"
#include "../tokenTypes/Literal.h"

class InsertAST : public ASTNode {
public:

    InsertAST(Identifier table, const std::vector<Identifier>& columns, const std::vector<Literal>& values);
    void performChecks() override;

    [[nodiscard]] const std::string &getTableName() const;
    [[nodiscard]] const std::vector<std::string> &getQualifiedColumns() const;
    [[nodiscard]] const std::vector<Literal> &getValues() const;

    [[nodiscard]] std::string repr() const;


private:
    Identifier table;
    std::vector<Identifier> columns;
    std::vector<Literal> values;

    std::string tableName;
    std::vector<std::string> qualifiedColumns;
};
