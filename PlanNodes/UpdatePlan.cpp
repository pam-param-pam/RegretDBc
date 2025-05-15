#include "../DataManager.h"
#include "UpdatePlan.h"
#include "fmt/base.h"

#include <utility>


UpdatePlan::UpdatePlan(const std::shared_ptr<PlanNodeBase> &source, std::string tableName, const std::vector<std::pair<std::string, Literal>>& assignments)
       : source(source), tableName(std::move(tableName)), assignments(assignments) {}

void UpdatePlan::execute() {
    source->execute();
    auto toUpdate = source->getResult();

    const auto& originalData = DataManager::getInstance().getTablesData(tableName);

    TypeHints::TableData newData;

    //Speed? What is speed?
    for (const auto& row : originalData) {
        bool shouldUpdate = false;

        for (const auto& updateRow : toUpdate) {
            if (row == updateRow) {
                shouldUpdate = true;
                break;
            }
        }
        if (shouldUpdate) {
            TypeHints::Row updatedRow = row;
            for (const auto& [column, literal] : assignments) {
                updatedRow[column] = literal;
            }
            newData.push_back(std::move(updatedRow));
        } else {
            newData.push_back(row);
        }
    }

    DataManager::getInstance().setTablesData(tableName, newData);
    DataManager::getInstance().setTablesData(tableName, newData);

}

TypeHints::TableData UpdatePlan::getResult() const {
    throw IntegrityError("Do not call getResult() on UpdatePlan");
}

std::string UpdatePlan::toString(int level) const {
    std::string result = "UpdatePlan(\n"; //todo assignennts

    result += std::string(level + 1, ' ') + "Table='" + tableName + "',\n";

    result += std::string(level + 1, ' ') + "Source=" + source->toString(level + 2) + "\n";

    result += std::string(level, ' ') + ")";

    return result;
}





