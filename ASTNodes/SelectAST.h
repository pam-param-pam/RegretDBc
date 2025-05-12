#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"
#include "../operators/Operand.h"
#include <memory>

class SelectAST : public ASTNode {
public:
    std::optional<Operand> whereExpr;

    SelectAST(const std::vector<Identifier>& columns, const std::vector<Identifier>& tables, std::optional<Operand> whereExpr, std::vector<std::pair<Identifier, std::string>> orderBy);

    void performChecks() override;

    [[nodiscard]] std::string repr() const;

    [[nodiscard]] const std::vector<std::string> &getTableNames() const;
    [[nodiscard]] const std::vector<std::string> &getQualifiedColumns() const;
    [[nodiscard]] const std::vector<std::pair<std::string, bool>> &getQualifiedOrderBy() const;

private:
    std::vector<std::string> tableNames;
    std::vector<std::string> qualifiedColumns;
    std::vector<std::pair<std::string, bool>> qualifiedOrderBy;
    std::vector<Identifier> columns;
    std::vector<Identifier> tables;
    std::vector<std::pair<Identifier, std::string>> orderBy;
};

