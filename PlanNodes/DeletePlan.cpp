#include "../DataManager.h"
#include "DeletePlan.h"

#include <utility>

DeletePlan::DeletePlan(const std::shared_ptr<PlanNodeBase>& source, std::string  tableName)
        : source(source), tableName(std::move(tableName)) {}

void DeletePlan::execute() {
    source->execute();
    auto toDelete = source->getResult();

    const auto& originalData = DataManager::getInstance().getTablesData(tableName);

    TypeHints::TableData newData;
    for (const auto& row : originalData) {
        if (std::find(toDelete.begin(), toDelete.end(), row) == toDelete.end()) {
            newData.push_back(row);
        }
    }

    DataManager::getInstance().setTablesData(tableName, newData);

}

TypeHints::TableData DeletePlan::getResult() const {
    throw IntegrityError("Do not call getResult() on DeletePlan");
}

std::string DeletePlan::toString(int level) const {
    std::string result = "DeletePlan(\n";

    result += std::string(level + 1, ' ') + "Table='" + tableName + "',\n";

    result += std::string(level + 1, ' ') + "Source=" + source->toString(level + 2) + "\n";

    result += std::string(level, ' ') + ")";

    return result;
}



