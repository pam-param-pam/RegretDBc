#include <sstream>
#include "InsertAST.h"
#include "fmt/ranges.h"


InsertAST::InsertAST(Identifier table, const std::vector<Identifier>& columns, const std::vector<Literal>& values)
        : table(std::move(table)), columns(columns), values(values) {}

void InsertAST::performChecks() {
    tableName = checkTable(table);

    qualifiedColumns = checkColumns({ tableName }, columns);

    DataManager::ColumnTypeMap columnTypeMap = DataManager::getInstance().getColumnTypesForTable(tableName);

    // Check if the types of values match the expected types for each column
    for (size_t i = 0; i < qualifiedColumns.size(); ++i) {
        const auto& qualifiedColumn = qualifiedColumns[i];
        const Literal::Type& valueType = values[i].getType();
        const Literal::Type& expectedType = columnTypeMap.at(qualifiedColumn);

        if (expectedType != valueType) {
            throw PreProcessorError("Type mismatch! Expected: " + Literal::typeToString(expectedType) + ", got: " + Literal::typeToString(valueType));
        }

    }

}
const std::string& InsertAST::getModifiedTableName() const {
    return tableName;
}

const std::vector<std::string>& InsertAST::getModifiedQualifiedColumns() const {
    return qualifiedColumns;
}

std::string InsertAST::repr() const {
    std::ostringstream ss;
    ss << "InsertStmt(table=" << table.value << ", columns=[";

    for (size_t i = 0; i < columns.size(); ++i) {
        ss << columns[i].value;
        if (i < columns.size() - 1) ss << ", ";
    }

    ss << "], values=[";

    for (size_t i = 0; i < values.size(); ++i) {
        ss << values[i].toString();
        if (i < values.size() - 1) ss << ", ";
    }

    ss << "])";
    return ss.str();
}
