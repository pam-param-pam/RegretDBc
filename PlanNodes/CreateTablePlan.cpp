#include "CreateTablePlan.h"
#include "../DataManager.h"

CreateTablePlan::CreateTablePlan(std::string name, const std::vector<std::string> &columns, std::vector<Literal::Type> &columnTypes)
     : name(std::move(name)), columns(columns), columnTypes(columnTypes) {}

void CreateTablePlan::execute() {
    DataManager& data_manager = DataManager::getInstance();
    DataManager::ColumnTypeMap col_types;


    for (size_t i = 0; i < columns.size(); ++i) {
        col_types[columns[i]] = columnTypes[i];
    }

    data_manager.addTable(name);
    data_manager.addColumnTypes(name, col_types);
}


