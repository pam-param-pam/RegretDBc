#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "tokenTypes/Literal.h"
#include "TypeHints.h"

class DataManager {
public:

    static DataManager& getInstance();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    bool doesTableExist(const std::string& table_name) const;

    std::vector<std::string> getColumnsForTable(const std::string& table_name) const;
    const TypeHints::ColumnTypeMap& getColumnTypesForTable(const std::string& table_name) const;
    const TypeHints::TableData& getTablesData(const std::string& table_name) const;
    std::vector<std::string> getAllTables() const;

    void insertRow(const std::string& table_name, const TypeHints::Row& row);

    void addTable(const std::string& table_name);
    void addColumnTypes(const std::string& tableName, const TypeHints::ColumnTypeMap& colTypes);
    void dropTable(const std::string& table_name);

    void setTablesData(const std::string &table_name, const TypeHints::TableData &newData);

    void addColumn(const std::string& tableName, const std::string& columnName, Literal::Type columnType);
    void dropColumn(const std::string& tableName, const std::string& columnName);
    void renameColumn(const std::string& tableName, const std::string& oldName, const std::string& newName);
    void modifyColumnType(const std::string& tableName, const std::string& columnName, Literal::Type newType);


private:
    DataManager() = default;

    std::unordered_map<std::string, TypeHints::ColumnTypeMap> columnTypes;
    std::unordered_map<std::string, TypeHints::TableData> tableData;
};
