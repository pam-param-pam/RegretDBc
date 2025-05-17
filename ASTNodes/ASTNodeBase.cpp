#include "ASTNodeBase.h"
#include "../utils.h"


void ASTNode::setSqlText(const std::string &sql) {
    sqlText = sql;
}

void ASTNode::verify() {
    try {
        performChecks();
    } catch (PreProcessorError &e) {
        if (sqlText.empty()) {
            throw RegretDBError("sqlText not set in ASTNode");
        }
        e.setMessage(e.getPrettyError(sqlText));
        throw e;
    }
}

std::string ASTNode::checkTable(const Identifier &table) {
    if (!DataManager::getInstance().doesTableExist(table.value)) {
        throw PreProcessorError("Table '" + table.value + "' not found.", table.value);
    }
    return table.value;
}

std::vector<std::string> ASTNode::checkTables(const std::vector<Identifier> &tables) {
    std::vector<std::string> seen;
    for (const auto &table: tables) {

        if (std::find(seen.begin(), seen.end(), table.value) != seen.end()) {
            throw PreProcessorError("Duplicate table '" + table.value + "' found.", table.value);
        }

        auto checkedTable = checkTable(table);
        seen.push_back(checkedTable);
    }

    return seen;
}


std::string ASTNode::checkColumn(std::vector<std::string> tables, const std::string &column) {
    auto [table_name, col_name] = splitColumn(column);
    bool flag = false;

    if (tables.size() == 1 && table_name.empty()) {
        table_name = tables[0];
        flag = true;
    }

    if (table_name.empty()) {
        throw PreProcessorError("Column '" + col_name + "' must be prefixed (ambiguity error)", col_name);
    }

    if (std::find(tables.begin(), tables.end(), table_name) == tables.end()) {
        throw PreProcessorError("Table '" + table_name + "' is not specified in 'FROM' clause", table_name);
    }

    auto columns = DataManager::getInstance().getColumnsForTable(table_name);
    std::string qualified_col_name = table_name + "." + col_name;

    if (std::find(columns.begin(), columns.end(), qualified_col_name) == columns.end()) {
        throw PreProcessorError("Column '" + col_name + "' not found in table '" + table_name + "'", col_name);
    }

    return flag ? qualified_col_name : column;
}

std::vector<std::string> ASTNode::checkColumns(const std::vector<std::string> &tables, const std::vector<Identifier> &columns) {
    std::vector<std::string> seen;

    for (const auto &column: columns) {
        auto columnName = column.value;
        if (std::find(seen.begin(), seen.end(), columnName) != seen.end()) {
            throw PreProcessorError("Duplicate column '" + columnName + "' found.", columnName);
        }

        auto qualified = checkColumn(tables, columnName);

        seen.push_back(qualified);
    }

    return seen;
}

void ASTNode::checkColumnType(TypeHints::ColumnTypeMap columnTypeMap, const std::string &qualifiedColumn, const Literal &literal) {
    const Literal::Type &valueType = literal.getType();
    const Literal::Type &expectedType = columnTypeMap.at(qualifiedColumn);

    if (expectedType != valueType && valueType != Literal::Type::NULL_VALUE) {
        throw PreProcessorError("Type mismatch! Expected: " + Literal::typeToString(expectedType) + ", got: " + Literal::typeToString(valueType), literal.toString());
    }
}

void ASTNode::checkWhereExpr(const std::vector<std::string> &tableNames, const std::optional<Condition> &whereExpr) {
    whereExpr->visitColumns([this, &tableNames](std::string &column, const std::optional<Literal> &literal) {
        column = checkColumn(tableNames, column);

        if (literal) {
            auto [tableName, colName] = splitColumn(column);
            const auto &columnTypeMap = DataManager::getInstance().getColumnTypesForTable(tableName);
            checkColumnType(columnTypeMap, column, *literal);
        }

    });
}
