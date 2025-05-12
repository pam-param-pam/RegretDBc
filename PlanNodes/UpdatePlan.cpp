#include "../DataManager.h"
#include "UpdatePlan.h"

#include <utility>

UpdatePlan::UpdatePlan(const std::shared_ptr<PlanNodeBase> &source, std::string tableName, const std::vector<std::pair<std::string, Literal>>& assignments)
       : source(source), tableName(std::move(tableName)), assignments(assignments) {}

void UpdatePlan::execute() {
//    source->execute();
//    auto toDelete = source->getResult();
//
//    const auto& originalData = DataManager::getInstance().getTablesData(tableName);
//
//    TypeHints::TableData newData;
//    for (const auto& row : originalData) {
//        if (std::find(toDelete.begin(), toDelete.end(), row) == toDelete.end()) {
//            newData.push_back(row);
//        }
//    }
//
//    DataManager::getInstance().setTablesData(tableName, newData);

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





