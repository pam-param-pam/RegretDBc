#include "InsertPlan.h"
#include "../DataManager.h"
InsertPlan::InsertPlan(std::string tableName, std::vector<std::string> columns, std::vector<Literal> values)
        : tableName(std::move(tableName)), columns(std::move(columns)), values(std::move(values)) {}

void InsertPlan::execute() {
    DataManager& data_manager = DataManager::getInstance();

    DataManager::Row row;
    for (size_t i = 0; i < columns.size(); ++i) {
        row[columns[i]] = values[i].getValue();
    }

    data_manager.insertRow(tableName, row);
}


