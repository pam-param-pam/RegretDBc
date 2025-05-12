#include "DataManager.h"
#include "fmt/base.h"

DataManager& DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

bool DataManager::doesTableExist(const std::string& table_name) const {
    return columnTypes.count(table_name) > 0;
}

std::vector<std::string> DataManager::getColumnsForTable(const std::string& table_name) const {
    std::vector<std::string> columns;
    const auto& col_map = columnTypes.at(table_name);
    for (const auto& [col, _] : col_map) {
        columns.push_back(col);
    }
    return columns;
}


const TypeHints::ColumnTypeMap& DataManager::getColumnTypesForTable(const std::string& table_name) const {
    return columnTypes.at(table_name);
}

const TypeHints::TableData& DataManager::getTablesData(const std::string& table_name) const {
    return tableData.at(table_name);
}

void DataManager::insertRow(const std::string& table_name, const TypeHints::Row& row) {
    tableData[table_name].push_back(row);
}

void DataManager::addTable(const std::string& table_name) {
    tableData[table_name] = {};
}

void DataManager::addColumnTypes(const std::string& table_name, const TypeHints::ColumnTypeMap& col_types) {
    columnTypes[table_name] = col_types;
}

void DataManager::dropTable(const std::string& table_name) {
    tableData.erase(table_name);
    columnTypes.erase(table_name);
}
void DataManager::setTablesData(const std::string& table_name, const TypeHints::TableData& newData) {
    tableData[table_name] = newData;
}
void DataManager::printTable(TypeHints::TableData data) {
    fmt::println("Printing project data ({} rows):", data.size());

    for (size_t rowIdx = 0; rowIdx < data.size(); ++rowIdx) {
        const auto& row = data[rowIdx];
        fmt::print("Row {}:\n", rowIdx);

        for (const auto& [key, val] : row) {
            Literal::Type type;

            if (std::holds_alternative<std::monostate>(val)) {
                type = Literal::Type::NULL_VALUE;
            } else if (std::holds_alternative<std::string>(val)) {
                type = Literal::Type::TEXT;
            } else if (std::holds_alternative<int>(val)) {
                type = Literal::Type::INTEGER;
            } else if (std::holds_alternative<bool>(val)) {
                type = Literal::Type::BOOLEAN;
            } else {
                throw std::runtime_error("Unsupported variant type");
            }

            Literal lit(type, val);
            fmt::print("  {} = {}\n", key, lit.toString());
        }
    }
}