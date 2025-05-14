#include "../DataManager.h"
#include "DropTablePlan.h"

DropTablePlan::DropTablePlan(std::string name)
        : name(std::move(name)) {}

void DropTablePlan::execute() {
    DataManager& dataManager = DataManager::getInstance();
    dataManager.dropTable(name);
}

TypeHints::TableData DropTablePlan::getResult() const {
    throw IntegrityError("Do not call getResult() on CreateTablePlan");
}

std::string DropTablePlan::toString(int level) const {
    std::string result = "DropTablePlan(\n";

    result += std::string(level + 1, ' ') + "Table='" + name + "',\n";

    result += "]\n" + std::string(level, ' ') + ")";
    return result;
}




