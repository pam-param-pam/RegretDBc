#pragma once

#include <string>
#include "../TypeHints.h"
#include "../operators/Condition.h"

///---------------- Table Scan ----------------

class TableScan : public PlanNodeBase {
public:
    explicit TableScan(std::string table);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::string table;
};


///---------------- Cross Join ----------------

class CrossJoin : public PlanNodeBase {
public:
    CrossJoin(std::unique_ptr<PlanNodeBase> left, std::unique_ptr<PlanNodeBase> right);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::unique_ptr<PlanNodeBase> left;
    std::unique_ptr<PlanNodeBase> right;

};

///---------------- Filter ----------------

class Filter : public PlanNodeBase {
public:
    Filter(std::unique_ptr<PlanNodeBase> source, Condition condition);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::unique_ptr<PlanNodeBase> source;
    Condition condition;

};


///---------------- Project ----------------

class Project : public PlanNodeBase {
public:
    Project(std::unique_ptr<PlanNodeBase> source, const std::vector<std::string> &columns);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::unique_ptr<PlanNodeBase> source;
    std::vector<std::string> columns;

};

///---------------- Sort ----------------

class Sort : public PlanNodeBase {
public:
    Sort(std::unique_ptr<PlanNodeBase> source, const std::vector<std::pair<std::string, bool>> &orderBy);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    std::unique_ptr<PlanNodeBase> source;
    std::vector<std::pair<std::string, bool>> orderBy;

};

///---------------- Visualize ----------------

class Visualize : public PlanNodeBase {
public:
    explicit Visualize(std::unique_ptr<PlanNodeBase> source);

    void execute() override;

    [[nodiscard]] std::string toString(int level) const override;

    [[nodiscard]] TypeHints::TableData getResult() const override;

private:
    void visualizeTable();

    std::unique_ptr<PlanNodeBase> source;
    std::vector<std::string> headers;
    TypeHints::TableData data;
};
