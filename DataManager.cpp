#include "DataManager.h"

DataManager& DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

bool DataManager::doesTableExist(const std::string& table_name) const {
    return column_types.count(table_name) > 0;
}

std::vector<std::string> DataManager::getColumnsForTable(const std::string& table_name) const {
    std::vector<std::string> columns;
    const auto& col_map = column_types.at(table_name);
    for (const auto& [col, _] : col_map) {
        columns.push_back(col);
    }
    return columns;
}


const DataManager::ColumnTypeMap& DataManager::getColumnTypesForTable(const std::string& table_name) const {
    return column_types.at(table_name);
}

const DataManager::TableData& DataManager::getTablesData(const std::string& table_name) const {
    return table_data.at(table_name);
}

void DataManager::insertRow(const std::string& table_name, const Row& row) {
    table_data[table_name].push_back(row);
}

void DataManager::addTable(const std::string& table_name) {
    table_data[table_name] = {};
}

void DataManager::addColumnTypes(const std::string& table_name, const ColumnTypeMap& col_types) {
    column_types[table_name] = col_types;
}

void DataManager::dropTable(const std::string& table_name) {
    table_data.erase(table_name);
    column_types.erase(table_name);
}
