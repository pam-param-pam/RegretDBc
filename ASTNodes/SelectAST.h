#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

// Define the SelectStmt class inheriting from ASTNode
class SelectAST : public ASTNode {
public:
    // Members for columns, tables, where expression, and order by
    std::vector<Identifier> columns;
    std::vector<Identifier> tables;
    std::string where_expr;
    std::string order_by;

    // Constructor
    SelectAST(const std::vector<Identifier>& columns, const std::vector<Identifier>& tables,
              std::string  where_expr, std::string order_by);

    // Override perform_checks method from ASTNode
    void performChecks() override;

    // Override the __repr__ method (using operator<< for printing in C++)
    [[nodiscard]] std::string repr() const;
};

