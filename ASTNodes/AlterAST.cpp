#include "AlterAST.h"
#include "fmt/format.h"


AlterAST::AlterAST(Action action, Identifier table, Identifier column, std::string newValue)
        : table(std::move(table)), action(action), column(std::move(column)), newValue(std::move(newValue)) {}

AlterAST::AlterAST(AlterAST::Action action, Identifier table, Identifier column)
        : table(std::move(table)), action(action), column(std::move(column)) {}

void AlterAST::performChecks() {
    tableName = checkTable(table);

    DataManager &dataManager = DataManager::getInstance();

    if (action == Action::ADD) {
        qualifiedColumn = tableName + "." + column.value;

        const auto &columns = dataManager.getColumnsForTable(tableName);
        if (std::find(columns.begin(), columns.end(), qualifiedColumn) != columns.end()) {
            throw PreProcessorError("Column: " + qualifiedColumn + " already exists for table: " + qualifiedColumn);
        }
    } else if (action == Action::DROP) {
        qualifiedColumn = checkColumn({tableName}, column.value);
    } else if (action == Action::RENAME) {
        qualifiedColumn = checkColumn({tableName}, column.value);
        newValue = tableName + "." + newValue;

    } else if (action == Action::MODIFY) {
        qualifiedColumn = checkColumn({tableName}, column.value);
        if (!dataManager.getTablesData(tableName).empty()) {
            throw PreProcessorError("Unable to modify column. Data exists for this table");
        }
    } else {
        throw IntegrityError("Unexpected Alter action type");
    }
}

const std::string &AlterAST::getQualifiedColumn() const {
    return qualifiedColumn;
}

const std::string &AlterAST::getNewValue() const {
    return newValue;
}

const std::string &AlterAST::getTableName() const {
    return tableName;
}

AlterAST::Action AlterAST::getAction() const {
    return action;
}

std::string AlterAST::actionToString(AlterAST::Action action1) {
    switch (action1) {
        case Action::ADD:
            return "ADD";
        case Action::DROP:
            return "DROP";
        case Action::RENAME:
            return "RENAME";
        case Action::MODIFY:
            return "MODIFY";
        default:
            throw IntegrityError("Unable to match action");
    }
}

std::string AlterAST::repr() const {
    std::string repr = fmt::format("AlterStmt(table={}, action={}, column={}, new_value={})", table.value, actionToString(action), column.value, newValue);
    return repr;
}
