#include "DataManager.h"

DataManager &DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

bool DataManager::doesTableExist(const std::string &tableName) const {
    return columnTypes.count(tableName) > 0;
}

std::vector<std::string> DataManager::getColumnsForTable(const std::string &tableName) const {
    std::vector<std::string> columns;
    const auto &colMap = columnTypes.at(tableName);
    for (const auto &[col, _]: colMap) {
        columns.push_back(col);
    }
    return columns;
}


const TypeHints::ColumnTypeMap &DataManager::getColumnTypesForTable(const std::string &tableName) const {
    return columnTypes.at(tableName);
}

const TypeHints::TableData &DataManager::getTablesData(const std::string &tableName) const {
    return tableData.at(tableName);
}

void DataManager::insertRow(const std::string &tableName, const TypeHints::Row &row) {
    tableData[tableName].push_back(row);
}

void DataManager::addTable(const std::string &tableName) {
    tableData[tableName] = {};
}

void DataManager::addColumnTypes(const std::string &tableName, const TypeHints::ColumnTypeMap &colTypes) {
    columnTypes[tableName] = colTypes;
}

void DataManager::dropTable(const std::string &tableName) {
    columnTypes.erase(tableName);
    tableData.erase(tableName);
}

void DataManager::setTablesData(const std::string &table_name, const TypeHints::TableData &newData) {
    tableData[table_name] = newData;
}

void DataManager::addColumn(const std::string &tableName, const std::string &columnName, Literal::Type columnType) {
    columnTypes[tableName][columnName] = columnType;

    for (auto &row: tableData[tableName]) {
        row[columnName] = Literal();  // Default null
    }
}

void DataManager::dropColumn(const std::string &tableName, const std::string &columnName) {
    columnTypes[tableName].erase(columnName);

    for (auto &row: tableData[tableName]) {
        row.erase(columnName);
    }
}

void DataManager::renameColumn(const std::string &tableName, const std::string &oldName, const std::string &newName) {
    auto &colMap = columnTypes[tableName];
    auto it = colMap.find(oldName);
    if (it != colMap.end()) {
        colMap[newName] = it->second;
        colMap.erase(it);
    }

    for (auto &row: tableData[tableName]) {
        auto valIt = row.find(oldName);
        if (valIt != row.end()) {
            row[newName] = valIt->second;
            row.erase(valIt);
        }
    }
}

void DataManager::modifyColumnType(const std::string &tableName, const std::string &columnName, Literal::Type newType) {
    columnTypes[tableName][columnName] = newType;
}

std::vector<std::string> DataManager::getAllTables() const {
    std::vector<std::string> tables;
    tables.reserve(columnTypes.size());
    for (const auto &[tableName, _]: columnTypes) {
        tables.push_back(tableName);
    }
    return tables;
}
