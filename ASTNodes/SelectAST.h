#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"
#include "../operators/Operand.h"
#include <memory>

// Define the SelectStmt class inheriting from ASTNode
class SelectAST : public ASTNode {
public:
    // Members for columns, tables, where expression, and order by
    std::vector<Identifier> columns;
    std::vector<Identifier> tables;
    std::shared_ptr<Operand> whereExpr;
    std::string order_by;

    // Constructor
    SelectAST(const std::vector<Identifier>& columns, const std::vector<Identifier>& tables, std::shared_ptr<Operand> whereExpr, std::string order_by);

    // Override perform_checks method from ASTNode
    void performChecks() override;

    // Override the __repr__ method (using operator<< for printing in C++)
    [[nodiscard]] std::string repr() const;

    [[nodiscard]] const std::vector<std::string> &getTableNames() const;
    [[nodiscard]] const std::vector<std::string> &getQualifiedColumns() const;
private:
    std::vector<std::string> tableNames;
    std::vector<std::string> qualifiedColumns;
};

