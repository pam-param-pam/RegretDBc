#include "CreateAST.h"
#include "fmt/ranges.h"


CreateAST::CreateAST(std::string table_name, const std::vector<Identifier> &columns, std::vector<Literal::Type> &columnTypes)
        : tableName(std::move(table_name)), columns(columns), columnTypes(columnTypes){}

void CreateAST::performChecks() {
    if (columns.size() != columnTypes.size()) {
        throw PreProcessorError("Mismatch between number of columns and column types.");
    }
    if (DataManager::getInstance().doesTableExist(tableName)) {
        throw PreProcessorError("Table '" + tableName + "' already exists");
    }

    for (const auto& column : columns) {
        qualifiedColumns.push_back(tableName + "." + column.value);
    }
}

const std::string &CreateAST::getTableName() const {
    return tableName;
}

const std::vector<std::string> &CreateAST::getQualifiedColumns() const {
    return qualifiedColumns;
}

const std::vector<Literal::Type> &CreateAST::getColumnTypes() const {
    return columnTypes;
}

std::string CreateAST::repr() const {
    std::string repr = fmt::format("CreateStmt(table={}, columns=[", tableName);

    for (auto i = 0; i < columns.size(); ++i) {
        repr += fmt::format("({}, {})", columns[i].value, Literal::typeToString(columnTypes[i]));
        if (i < columns.size() - 1) {
            repr += ", ";
        }
    }
    repr += "]";
    return repr;
}
