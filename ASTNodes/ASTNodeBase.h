#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include "../DataManager.h"
#include <sstream>
#include <memory>

#include "../exceptions/exceptions.h"
#include "fmt/base.h"
#include "../tokenTypes/Identifier.h"
#include "../operators/Operand.h"

class ASTNode {
protected:
    std::string sql_text;
    std::string checkTable(const Identifier &table);
    std::vector<std::string> checkTables(const std::vector<Identifier>& tables);
    std::string checkColumn(std::vector<std::string> tables, const std::string& column);
    std::vector<std::string> checkColumns(const std::vector<std::string> &tables, const std::vector<Identifier> &columns);
    void checkColumnType(TypeHints::ColumnTypeMap columnTypeMap, const std::string &qualifiedColumn, const Literal &literal);
    void checkWhereExpr(const std::vector<std::string> &tableNames, const std::optional<Operand> &whereExpr);

public:
    ASTNode() = default;

    virtual ~ASTNode() = default;

    void setSqlText(const std::string& sql);
    void verify();

    virtual void performChecks() = 0;
    [[nodiscard]] virtual std::string repr() const = 0;
};
