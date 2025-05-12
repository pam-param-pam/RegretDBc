#pragma once

#include <string>
#include <vector>
#include "PlanNodeBase.h"
#include "../operators/Operand.h"


class UpdatePlan : public PlanNodeBase {
public:
    explicit UpdatePlan(const std::shared_ptr<PlanNodeBase>& source, std::string tableName, const std::vector<std::pair<std::string, Literal>>& assignments);

    void execute() override;
    [[nodiscard]] std::string toString(int level) const override;
    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string tableName;
    std::shared_ptr<PlanNodeBase> source;
    const std::vector<std::pair<std::string, Literal>>& assignments;

};
