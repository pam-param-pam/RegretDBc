#pragma once

#include <string>
#include <vector>
#include "../tokenTypes/Identifier.h"
#include "../TypeHints.h"
#include "../operators/Condition.h"
#include "../DataManager.h"

class ASTNode {
protected:
    std::string sqlText;

    std::string checkTable(const Identifier &table);

    std::vector<std::string> checkTables(const std::vector<Identifier> &tables);

    std::string checkColumn(std::vector<std::string> tables, const std::string &column);

    std::vector<std::string> checkColumns(const std::vector<std::string> &tables, const std::vector<Identifier> &columns);

    void checkColumnType(TypeHints::ColumnTypeMap columnTypeMap, const std::string &qualifiedColumn, const Literal &literal);

    void checkWhereExpr(const std::vector<std::string> &tableNames, const std::optional<Condition> &whereExpr);

public:
    ASTNode() = default;

    virtual ~ASTNode() = default;

    void setSqlText(const std::string &sql);

    void verify();

    virtual void performChecks() = 0;

    [[nodiscard]] virtual std::string repr() const = 0;
};
