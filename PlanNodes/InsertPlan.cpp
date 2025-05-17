#include "InsertPlan.h"
#include "../DataManager.h"


InsertPlan::InsertPlan(std::string tableName, std::vector<std::string> columns, std::vector<Literal> values)
        : tableName(std::move(tableName)), columns(std::move(columns)), values(std::move(values)) {}

void InsertPlan::execute() {
    DataManager &dataManager = DataManager::getInstance();

    TypeHints::Row row;
    for (auto i = 0; i < columns.size(); ++i) {
        row[columns[i]] = values[i];
    }

    dataManager.insertRow(tableName, row);
}

TypeHints::TableData InsertPlan::getResult() const {
    throw IntegrityError("Do not call getResult() on InsertPlan");
}

std::string InsertPlan::toString(int level) const {
    std::string result = "InsertPlan(\n";

    result += std::string(level + 1, ' ') + "Table='" + tableName + "',\n";

    result += std::string(level + 1, ' ') + "Columns=[";
    for (auto i = 0; i < columns.size(); ++i) {
        result += columns[i];
        if (i < columns.size() - 1) {
            result += ", ";
        }
    }
    result += "],\n";

    result += std::string(level + 1, ' ') + "Values=[";
    for (auto i = 0; i < values.size(); ++i) {
        result += values[i].toString();
        if (i < values.size() - 1) {
            result += ", ";
        }
    }

    result += "]\n" + std::string(level, ' ') + ")";

    return result;
}
