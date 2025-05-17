#pragma once

#include <string>
#include <vector>
#include "ASTNodeBase.h"

class CreateAST : public ASTNode {
public:
    CreateAST(std::string table_name, const std::vector<Identifier> &columns, std::vector<Literal::Type> &columnTypes);

    void performChecks() override;

    [[nodiscard]] std::string repr() const override;

    [[nodiscard]] const std::string &getTableName() const;

    [[nodiscard]] const std::vector<std::string> &getQualifiedColumns() const;

    [[nodiscard]] const std::vector<Literal::Type> &getColumnTypes() const;

private:
    std::vector<Identifier> columns;
    std::string tableName;
    std::vector<Literal::Type> columnTypes;
    std::vector<std::string> qualifiedColumns;
};
