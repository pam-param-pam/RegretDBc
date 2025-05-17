#include "InsertAST.h"


InsertAST::InsertAST(Identifier table, const std::vector<Identifier> &columns, const std::vector<Literal> &values)
        : table(std::move(table)), columns(columns), values(values) {}

void InsertAST::performChecks() {
    tableName = checkTable(table);
    qualifiedColumns = checkColumns({tableName}, columns);

    const auto &columnTypeMap = DataManager::getInstance().getColumnTypesForTable(tableName);

    if (columns.size() != values.size()) {
        throw PreProcessorError("Number of columns does not match number of values");
    }
    if (columns.size() != columnTypeMap.size()) {
        throw PreProcessorError("Number of columns does not match number of columns in the actual talbe");
    }

    // Check if the types of values match the expected types for each column
    for (auto i = 0; i < qualifiedColumns.size(); ++i) {
        checkColumnType(columnTypeMap, qualifiedColumns[i], values[i]);

    }

}

const std::string &InsertAST::getTableName() const {
    return tableName;
}

const std::vector<std::string> &InsertAST::getQualifiedColumns() const {
    return qualifiedColumns;
}

const std::vector<Literal> &InsertAST::getValues() const {
    return values;
}

std::string InsertAST::repr() const {
    std::string result = "InsertStmt(table=" + table.value + ", columns=[";

    for (auto i = 0; i < columns.size(); ++i) {
        result += columns[i].value;
        if (i < columns.size() - 1)
            result += ", ";
    }
    result += "], values=[";

    for (auto i = 0; i < values.size(); ++i) {
        result += values[i].toString();
        if (i < values.size() - 1)
            result += ", ";
    }

    result += "])";
    return result;
}
