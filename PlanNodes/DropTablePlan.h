#pragma once

#include "PlanNodeBase.h"
#include <string>
#include <vector>
#include <utility>

class DropTablePlan : public PlanNodeBase {
public:
    explicit DropTablePlan(std::string name);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string name;
};
