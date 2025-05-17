#pragma once

#include <string>
#include <vector>
#include "PlanNodeBase.h"
#include "../operators/Condition.h"


class UpdatePlan : public PlanNodeBase {
public:
    explicit UpdatePlan(std::unique_ptr<PlanNodeBase> source, std::string tableName, const std::vector<std::pair<std::string, Literal>> &assignments);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string tableName;
    std::unique_ptr<PlanNodeBase> source;
    const std::vector<std::pair<std::string, Literal>> &assignments;
};
