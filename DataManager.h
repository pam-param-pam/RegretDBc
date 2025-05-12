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

    bool doesTableExist(const std::string& table_name) const;

    std::vector<std::string> getColumnsForTable(const std::string& table_name) const;
    const TypeHints::ColumnTypeMap& getColumnTypesForTable(const std::string& table_name) const;
    const TypeHints::TableData& getTablesData(const std::string& table_name) const;

    void insertRow(const std::string& table_name, const TypeHints::Row& row);

    void addTable(const std::string& table_name);
    void addColumnTypes(const std::string& table_name, const TypeHints::ColumnTypeMap& col_types);
    void dropTable(const std::string& table_name);

private:
    DataManager() = default;

    std::unordered_map<std::string, TypeHints::ColumnTypeMap> columnTypes;
    std::unordered_map<std::string, TypeHints::TableData> tableData;


};
