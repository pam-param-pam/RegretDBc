#include "DataManager.h"

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
