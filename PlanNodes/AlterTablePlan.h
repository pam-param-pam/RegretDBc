#pragma once

#include <string>
#include <memory>
#include "../TypeHints.h"
#include "../ASTNodes/AlterAST.h"
#include "PlanNodeBase.h"

class AlterTablePlan : public PlanNodeBase {
public:
    AlterTablePlan(AlterAST::Action action, std::string tableName, std::string columnName, std::string newValue = "");

    void execute() override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

    [[nodiscard]] std::string toString(int level) const override;

private:
    AlterAST::Action action;
    std::string tableName;
    std::string columnName;
    std::string newValue;

    void addColumn();
    void dropColumn();
    void renameColumn();
    void modifyColumn();
};
