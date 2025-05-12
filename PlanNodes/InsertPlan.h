#pragma once

#include "PlanNodeBase.h"
#include <string>
#include <vector>

class InsertPlan : public PlanNodeBase {
public:
    InsertPlan(std::string tableName, std::vector<std::string> columns, std::vector<Literal> values);
    void execute() override;
    [[nodiscard]] std::string toString(int level) const override;
    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<Literal> values;
};
