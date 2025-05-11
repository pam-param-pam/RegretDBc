#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "tokenTypes/Literal.h"

class DataManager {
public:
    using Row = std::unordered_map<std::string, std::variant<std::string, int, bool>>;
    using TableData = std::vector<Row>;
    using ColumnTypeMap = std::unordered_map<std::string, Literal::Type>;

    static DataManager& getInstance();

    bool doesTableExist(const std::string& table_name) const;

    std::vector<std::string> getColumnsForTable(const std::string& table_name) const;
    const ColumnTypeMap& getColumnTypesForTable(const std::string& table_name) const;
    const TableData& getTablesData(const std::string& table_name) const;

    void insertRow(const std::string& table_name, const Row& row);

    void addTable(const std::string& table_name);
    void addColumnTypes(const std::string& table_name, const ColumnTypeMap& col_types);
    void dropTable(const std::string& table_name);

private:
    DataManager() = default;

    std::unordered_map<std::string, ColumnTypeMap> column_types;
    std::unordered_map<std::string, TableData> table_data;


};
