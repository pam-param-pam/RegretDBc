#include "CreateTablePlan.h"
#include "../DataManager.h"

CreateTablePlan::CreateTablePlan(std::string name, const std::vector<std::string> &columns, std::vector<Literal::Type> &columnTypes)
     : name(std::move(name)), columns(columns), columnTypes(columnTypes) {}

void CreateTablePlan::execute() {
    DataManager& data_manager = DataManager::getInstance();
    TypeHints::ColumnTypeMap col_types;


    for (size_t i = 0; i < columns.size(); ++i) {
        col_types[columns[i]] = columnTypes[i];
    }

    data_manager.addTable(name);
    data_manager.addColumnTypes(name, col_types);
}

TypeHints::TableData CreateTablePlan::getResult() const {
    throw IntegrityError("Do not call getResult() on CreateTablePlan");
}

std::string CreateTablePlan::toString(int level) const {
    std::string result = "CreateTablePlan(\n";

    result += std::string(level + 1, ' ') + "Table='" + name + "',\n";

    result += std::string(level + 1, ' ') + "Columns=[";
    for (auto i = 0; i < columns.size(); ++i) {
        result += columns[i] + ": " + Literal::typeToString(columnTypes[i]);
        if (i < columns.size() - 1) {
            result += ", ";
        }
    }
    result += "]\n" + std::string(level, ' ') + ")";
    return result;
}




