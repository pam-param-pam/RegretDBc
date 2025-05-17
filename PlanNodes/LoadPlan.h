#pragma once

#include "PlanNodeBase.h"
#include <string>
#include <vector>
#include <utility>

class LoadPlan : public PlanNodeBase {
public:
    explicit LoadPlan(std::string filePath);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string filePath;
};
