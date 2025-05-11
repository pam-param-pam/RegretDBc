#include "ASTNodeBase.h"
#include "../exceptions/exceptions.h"
#include "../DataManager.h"
#include "../tokenTypes/Identifier.h"
#include <algorithm>

ASTNode::ASTNode() {}

void ASTNode::setSqlText(const std::string& sql) {
    sql_text = sql;
}

void ASTNode::verify() {
    try {
        performChecks();
    } catch (PreProcessorError& e) {
        if (sql_text.empty()) {
            throw RegretDBError("sql_text not set in ASTNode");
        }
        e.sql_stmt = sql_text;
        throw e;
    }
}

std::string ASTNode::checkTable(const Identifier& table) {
    if (!DataManager::getInstance().doesTableExist(table.value)) {
        throw PreProcessorError("Table '" + table.value + "' not found.");
    }
    return table.value;
}

std::vector<std::string> ASTNode::checkTables(const std::vector<Identifier>& tables) {
    std::vector<std::string> seen;
    for (const auto& table : tables) {

        if (std::find(seen.begin(), seen.end(), table.value) != seen.end()) {
            throw PreProcessorError("Duplicate table '" + table.value + "' found.", table.value);
        }

        auto checkedTable = checkTable(table);
        seen.push_back(checkedTable);
    }

    return seen;
}

std::pair<std::string, std::string> ASTNode::splitColumn(const std::string& column) {
    size_t dot_pos = column.find('.');
    if (dot_pos != std::string::npos) {
        return { column.substr(0, dot_pos), column.substr(dot_pos + 1) };
    } else {
        return { "", column };
    }
}

std::string ASTNode::checkColumn(std::vector<std::string> tables, const std::string& column) {
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

std::vector<std::string> ASTNode::checkColumns(const std::vector<std::string>& tables, const std::vector<Identifier>& columns) {
    std::vector<std::string> seen;

    for (const auto& column : columns) {
        auto columnName = column.value;
        if (std::find(seen.begin(), seen.end(), columnName) != seen.end()) {
            throw PreProcessorError("Duplicate column '" + columnName + "' found.", columnName);
        }

        auto qualified = checkColumn(tables, columnName);

        seen.push_back(qualified);
    }

    return seen;
}
