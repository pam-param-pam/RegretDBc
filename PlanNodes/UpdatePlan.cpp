#include "../DataManager.h"
#include "UpdatePlan.h"


UpdatePlan::UpdatePlan(std::unique_ptr<PlanNodeBase> source, std::string tableName, const std::vector<std::pair<std::string, Literal>> &assignments)
        : source(std::move(source)), tableName(std::move(tableName)), assignments(assignments) {}

void UpdatePlan::execute() {
    source->execute();
    auto toUpdate = source->getResult();

    const auto &originalData = DataManager::getInstance().getTablesData(tableName);

    TypeHints::TableData newData;

    //Speed? What is speed?
    for (const auto &row: originalData) {
        bool shouldUpdate = false;

        for (const auto &updateRow: toUpdate) {
            if (row == updateRow) {
                shouldUpdate = true;
                break;
            }
        }
        if (shouldUpdate) {
            TypeHints::Row updatedRow = row;
            for (const auto &[column, literal]: assignments) {
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
    std::string indent(level + 1, ' ');
    std::string result = "UpdatePlan(\n";

    result += indent + "Table='" + tableName + "',\n";

    result += indent + "Assignments=[";
    for (size_t i = 0; i < assignments.size(); ++i) {
        const auto &[column, literal] = assignments[i];
        result += column + "=" + literal.toString();
        if (i < assignments.size() - 1) {
            result += ", ";
        }
    }
    result += "],\n";

    result += indent + "Source=" + source->toString(level + 2) + "\n";

    result += std::string(level, ' ') + ")";
    return result;
}
