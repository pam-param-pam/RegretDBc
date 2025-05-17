#pragma once

#include <string>
#include <vector>
#include "PlanNodeBase.h"
#include "../operators/Condition.h"


class DeletePlan : public PlanNodeBase {
public:
    explicit DeletePlan(std::unique_ptr<PlanNodeBase> source, std::string tableName);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string tableName;
    std::unique_ptr<PlanNodeBase> source;

};
