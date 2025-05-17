#include "AlterTablePlan.h"
#include "fmt/core.h"

AlterTablePlan::AlterTablePlan(AlterAST::Action action, std::string tableName, std::string columnName, std::string newValue)
        : action(action), tableName(std::move(tableName)), columnName(std::move(columnName)), newValue(std::move(newValue)) {}

void AlterTablePlan::execute() {

    switch (action) {
        case AlterAST::Action::ADD:
            addColumn();
            break;
        case AlterAST::Action::DROP:
            dropColumn();
            break;
        case AlterAST::Action::RENAME:
            renameColumn();
            break;
        case AlterAST::Action::MODIFY:
            modifyColumn();
            break;
        default:
            throw IntegrityError("Unexpected Alter action in plan");
    }
}

void AlterTablePlan::addColumn() {
    auto colType = Literal::getTypeFromValue(newValue);
    DataManager::getInstance().addColumn(tableName, columnName, colType);
}

void AlterTablePlan::dropColumn() {
    DataManager::getInstance().dropColumn(tableName, columnName);
}

void AlterTablePlan::renameColumn() {
    if (newValue.empty()) {
        throw IntegrityError("New column name must be provided for rename operation");
    }
    DataManager::getInstance().renameColumn(tableName, columnName, newValue);
}

void AlterTablePlan::modifyColumn() {
    auto colType = Literal::getTypeFromValue(newValue);
    DataManager::getInstance().modifyColumnType(tableName, columnName, colType);
}

TypeHints::TableData AlterTablePlan::getResult() const {
    throw IntegrityError("getResult() not supported for AlterPlan");
}

std::string AlterTablePlan::toString(int level) const {
    std::string indent(level * 2, ' ');
    std::string repr = fmt::format("{}AlterPlan(\n", indent);
    repr += fmt::format("{}  Table='{}',\n", indent, tableName);
    repr += fmt::format("{}  Action='{}',\n", indent, AlterAST::actionToString(action));
    repr += fmt::format("{}  Column='{}',\n", indent, columnName);
    if (!newValue.empty()) {
        repr += fmt::format("{}  NewValue='{}',\n", indent, newValue);
    }

    return repr;
}
