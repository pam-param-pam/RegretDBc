#pragma once

#include <string>
#include <vector>
#include "PlanNodeBase.h"


class DeletePlan : public PlanNodeBase {
public:
    explicit DeletePlan(std::string tableName);

    void execute() override;

private:
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<Literal::Type> columnTypes;
};
