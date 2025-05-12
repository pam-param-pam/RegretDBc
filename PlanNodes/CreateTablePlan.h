#pragma once

#include "PlanNodeBase.h"
#include <string>
#include <vector>
#include <utility>

class CreateTablePlan : public PlanNodeBase {
public:
    CreateTablePlan(std::string name, const std::vector<std::string> &columns, std::vector<Literal::Type> &columnTypes);
    void execute() override;
    [[nodiscard]] std::string toString(int level) const override;
    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string name;
    std::vector<std::string> columns;
    std::vector<Literal::Type> columnTypes;
};
