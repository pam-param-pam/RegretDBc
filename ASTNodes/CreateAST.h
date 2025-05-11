#pragma once

#include <string>
#include <vector>
#include <utility>  // for std::pair
#include "ASTNodeBase.h"

class CreateAST : public ASTNode {
public:
    CreateAST(std::string table_name, const std::vector<Identifier> &columns, std::vector<Literal::Type> &columnTypes);

    std::string name;
    std::vector<Identifier> columns;
    std::vector<Literal::Type> columnTypes;

    void performChecks() override;
    [[nodiscard]] std::string repr() const override;
    std::vector<std::string> qualifiedColumns;

};
